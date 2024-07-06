/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/06 10:57:50 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	

		while (true)
		{
			httpServer.startPoll();
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
