/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/12 15:42:08 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./server/Server.hpp"
#include "./test/test.hpp"

#include "./parsing/Parser.hpp"

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

		std::vector<ServerConf>::iterator serverIt = configuration.getHttp().getServer().begin();
		while (serverIt != configuration.getHttp().getServer().end()) {
			// Server httpServer;
			Server s((*serverIt).getSettings());
			
			// Server.setPortAvailable();
			// Server.bindSocket();
			// Server.listenToRequests();

			// while (true)
			// {
			// 	Server.startPoll2();
			// 	Server.printConnectedSockets();
			// }
			serverIt++;
		}
		
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
