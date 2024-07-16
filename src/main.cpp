/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 15:56:18 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./server/Server.hpp"
#include "./test/test.hpp"

#include "./parsing/Parser.hpp"

int main ( int argc, char** argv ) {
	try {
		Parser configuration(argc, argv);
		configuration.displayConf();
		std::cout << "---------------------------------------------------------" << std::endl;

		// std::vector<ServerConf>::iterator serverIt = configuration.getHttp().getServer().begin();
		// while (serverIt != configuration.getHttp().getServer().end()) {
			// 1) ServConf - holds cnofiguration
			//	Prepare Server instance ---> holds vector<Host>

			// Server httpServer;
			// Server s((*serverIt).getSettings());

			// s.setPortAvailable();
			// s.bindSocket();
			// s.listenToRequests();

			// while (true)
			// {
			// 	s.startPoll2();
			// 	s.printConnectedSockets();
			// }
		// 	serverIt++;
		// }
	} catch (std::exception& e) {
		std::cerr << R("Error: ") << e.what() << std::endl;
		return (1);
	}
	// } catch(Exception const &exception) {
	// 	std::cerr << RED << exception.what() << RESET << std::endl;
	// 	return exception.getError();
	// }

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
