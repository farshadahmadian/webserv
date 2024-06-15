/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 11:38:09 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/15 11:43:30 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <netdb.h> // for structs like "struct addrinfo"
#include <iostream>
// #include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa() ==> must be removed and ntohs()

void createServer(void) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	// hints.ai_family = AF_INET; // IPv4
	// hints.ai_family = AF_INET6; // IPv6
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo *res;
	
	getaddrinfo("127.0.0.1", "80", &hints, &res); // "res" will be the result which is an allocated linked list (must be deleted)

	struct addrinfo *temp;

	for (temp = res; temp != NULL; temp = res->ai_next) {
		std::cout << "ip address in network byte order = " << ((sockaddr_in *)temp->ai_addr)->sin_addr.s_addr << std::endl;
		std::cout << "ip address in host byte order = " << inet_ntoa(((sockaddr_in *)temp->ai_addr)->sin_addr) << std::endl;

		std::cout << "port number in network byte order = " << ((sockaddr_in *)temp->ai_addr)->sin_port << std::endl;
		std::cout << "port number in host byte order = " << ntohs(((sockaddr_in *)temp->ai_addr)->sin_port) << std::endl;
	}

	return;
}