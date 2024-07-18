/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 14:54:47 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./server/Server.hpp"
#include "./parsing/Parser.hpp"

int main ( int argc, char** argv ) {
	try {
		Parser configuration(argc, argv);
		configuration.displayConf();
		std::cout << "---------------------------------------------------------" << std::endl;

		std::vector<ServerConf>::const_iterator serverConfIt = configuration.getHttp().getServer().begin();
		std::vector<Server> servers;

		// 1) MERGING SERVERS WITH SAME PORT
		while (serverConfIt != configuration.getHttp().getServer().end()) {
			if (servers.size() > 0) {
				std::map<std::string, std::string> serverSettings = (*serverConfIt).getSettings();
				// Server* matched = isPortAlreadyInUse(serverSettings["port"], servers); --- returns the reference of the matched server
				// if (matched) { // check if the port is already in use
				//	merge the current server Configuration Block to the matched one
				//	serverConfIt++;
				//	continue ;
				// }
			}
			// Server s((*serverConfIt).getSettings());
			// servers.push_back(s);
			serverConfIt++;
		}

		/*
			class Poll {
				public:
					std::vector<Server> server;
					struct pollfd fd[MAX_CONNECTION * server.size()];

					void handleServer( void ) {
						int return_value = poll(pollFd...)

						if (return_value == something)
							handle_event()
						if (return_value == something)
							handle_event()
					}
			}
		*/

		// 2) START THE SERVERS BY ITERATING THE VECTOR
		/*
			std::vector<Server>::iterator serverIt = servers.begin();
			while (serverIt != servers.end()) {
				(*serverIt).setPortAvailable();
				(*serverIt).bindSocket();
				(*serverIt).listenToRequests();
				serverIt++;
			}

			while (true) {
				1) poll(pollFd, this->_monitoredFdsNum, 3000);

			}
		*/

		// while (true)
		// {
		// 	s.startPoll2();
		// 	s.printConnectedSockets();
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
