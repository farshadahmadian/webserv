/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 10:00:28 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "HttpConf.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"
#include "HttpServer.hpp"
#include "test.hpp"
#include "Logger.hpp"
#include "Parser.hpp"

/*  
	- Check verbosity of the parser: displayConf can be verbose=1 while parser steps are verbose=2
	- with verbose=0 there is not output for the configuration
	- when default settings are choosen, warn the user
*/

int main ( int argc, char** argv ) {
	try {
		Parser configuration(argc, argv);
		configuration.displayConf();
		std::cout << "---------------------------------------------------------" << std::endl;


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
		Conf configuration(argc, argv);
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
