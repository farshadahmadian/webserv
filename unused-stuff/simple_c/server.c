/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:10:06 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/11 15:43:54 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SOCKETS */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>		// inet_ntop
#include <netinet/in.h>

/* C LIBS */
#include <signal.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>			// fork
#include <stdlib.h>			// exit
#include <sys/wait.h>

#define PORT	"8000"
#define BACKLOG	10

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    // int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    // errno = saved_errno;
}

int main ( void ) {
	int rv;
	int	socket_fd;
	struct addrinfo hints;
	struct addrinfo *servInfo;

	// (1) GIVE SOME INSTRUCTION FOR THE SOCKET'S ADDRESS
	bzero(&hints, sizeof hints );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// hints.ai_protocol = 0;

	rv = getaddrinfo(NULL, PORT, &hints, &servInfo);
	if (rv != 0) {
		printf("[ERROR]: %s\n", gai_strerror(rv));
		return (1);
	}

	// (2) FIND THE FIRST SOCKET
	bool yes = true;
	struct addrinfo *p;
	for (p = servInfo; p != NULL; p = p->ai_next) {
		socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (socket_fd == -1) {
			printf("[ERROR]");
			continue ;
		}

		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			printf("[ERROR]");
			return (1); // system error
		}

		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) { // Associate the socket to the pre-specified port
			printf("[ERROR]");
			close(socket_fd);
			continue ;
		}
		break ;
	}
	freeaddrinfo(servInfo);

	// 3) START TO LISTEN
	if (p == NULL) {
		printf("[ERROR]: failed to bind\n");
		return (1);
	}

	if (listen(socket_fd, BACKLOG) == -1) {
		printf("[ERROR]: failed to listen\n");
		return (1);
	}

    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


	// 4) ACCEPTINW/WAITING for CONNECTIONS
	int client_fd;
	struct sockaddr_storage clientAddr;
	socklen_t client_size;
	while (true) {
		printf("* Waiting for connecitons...\n");
		client_size = sizeof clientAddr;
		client_fd = accept(socket_fd, (struct sockaddr *)&clientAddr, &client_size);
		if (client_fd == -1) {
			printf("[ERROR]: failed to accept\n");
			continue ;
		}

		printf("* Connection accepted...\n");
		rv = fork();
		if (rv == -1) //error
			printf("[ERROR]: failed to fork\n");
		else if (rv == 0) { // child
			close(socket_fd);
			if (send(client_fd, "HeyYou!\n", 8, 0) == -1)
				printf("[ERROR]: failed to send\n");
			close(client_fd);
			exit (0);
		}
		else { // parent
			close (client_fd);
		}
	}
	return (0);
}
