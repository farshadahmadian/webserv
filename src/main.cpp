/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 09:08:38 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "colors.h"
#include "Http.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"
#include "HttpServer.hpp"
#include "test.hpp"
#include "Logger.hpp"

int main ( int argc, char** argv ) {
	try {
		Conf settings(argc, argv);
		VERBOSE >= 1 ? settings.displayConf() : void();
		std::cout << "---------------------------------------------------------" << std::endl;

		settings.start();

		// HttpServer httpServer;
		// httpServer.setPortAvailable();
		// httpServer.bindSocket();
		// httpServer.listenToRequests();

		// while (true)
		// {
		// 	httpServer.startPoll2();
		// 	httpServer.printConnectedSockets();
		// }
		
	} catch(Exception const &exception) {
		std::cerr << RED << exception.what() << RESET << std::endl;
		return exception.getError();
	}

	// memoryTest();

	/* try {
		Conf settings(argc, argv);
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
