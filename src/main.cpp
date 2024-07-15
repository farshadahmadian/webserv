/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 16:16:19 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./server/Server.hpp"
#include "./parsing/Parser.hpp"

/*  
	- Check verbosity of the parser: displayConf can be verbose=1 while parser steps are verbose=2
	- with verbose=0 there is not output for the configuration
	- when default settings are choosen, warn the user
*/

int main ( int argc, char** argv ) {
	try {
		Parser configuration(argc, argv);
		// configuration.displayConf();
		std::cout << "---------------------------------------------------------" << std::endl;

		std::vector<ServerConf>::iterator serverIt = configuration.getHttp().getServer().begin();
		while (serverIt != configuration.getHttp().getServer().end()) {
			// Server httpServer;
			Server s((*serverIt).getSettings());
			
			s.setPortAvailable();
			s.bindSocket();
			s.listenToRequests();

			while (true)
			{
				s.startPoll2();
				// s.printConnectedSockets();
			}
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
