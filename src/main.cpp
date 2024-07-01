/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/01 09:57:42 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include "webserv.h"
#include "colors.h"
#include "Http.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"
#include "HttpServer.hpp"
#include "test.hpp"

int main ( int argc, char** argv ) {

	(void)argc;
	(void)argv;

	try {
		HttpServer httpServer;
		httpServer.setPortAvailable();
		httpServer.bindSocket();
		httpServer.listenToRequests();

		struct pollfd pollFds[1];
		pollFds[0].fd = httpServer.getListeningSocket().getSocketFd();
		pollFds[0].events = POLLIN; // when the socket is ready to be read (when the listening socket receives an incoming connection request)

		while (true)
		{
			int eventsNum = poll(pollFds, 1, 5000);
			if (eventsNum == 0)
				std::cout << "Timeout! No event received" << std::endl;
			else {
				// int readyToBeRead = pollFds[0].revents & POLLIN;
				int readyToBeRead = pollFds[0].revents;

				if (readyToBeRead != 0)	{
					std::cout << "Listening socket wiht fd " << pollFds[0].fd << " has data to be read" << std::endl;
					httpServer.acceptFirstRequestInQueue();
				}
				else {
					std::cout << "execption" << std::endl;
				}

				std::cout << "pollFds->events = " << pollFds->events << std::endl;
				std::cout << "pollFds->revents = " << (pollFds->revents & POLLIN) << std::endl;
			}
			
			httpServer.printConnectedSockets();
		}

		
	} catch(Exception const &exception) {
		std::cerr << RED << exception.what() << RESET << std::endl;
		return exception.getError();
	}

	// memoryTest();

	/* try {
		Conf webserver(argc, argv);
		// Conf conf(argc, argv);
		// Http http(conf);
		// http.startWebServ();
		//
		//
		//
		//
	} catch (std::exception& e) {
		std::cerr << R("Error: ") << e.what() << std::endl;
		return (1);
	} */
	return (0);
}
