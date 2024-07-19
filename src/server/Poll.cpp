/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/19 16:30:29 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

/*	Taking the configuration of Parser class, the Server instances are created
	only if there are not server with the same port. If there are server block
	with same port, they ar merged together.
*/
Poll::Poll( const Parser& configuration ):
	_serverList(std::vector<Server>()),
	_totalMonitored(0) {
	std::vector<ServerConf>::const_iterator serverConfIt = configuration.getHttp().getServer().begin();

	while (serverConfIt != configuration.getHttp().getServer().end()) {
		if (_serverList.size() > 0) {
			std::map<std::string, std::string> currentServerSettings = (*serverConfIt).getSettings();
			if (isPortAlreadyInUse(currentServerSettings)) { // check if the port is already in use
				serverConfIt++;
				continue ;
			}
		}
		// Server s = new Server((*serverConfIt).getSettings());
		Server s((*serverConfIt).getSettings());
		_serverList.push_back(s); // ------ crash in Mac but not in Linux ------- THE MOST MISTERIOUS PROBLEM IN THE HISTORY OF CODING  -------------------- !!!!
		serverConfIt++;
	}
}
Poll::~Poll( void ) {};

/*	Check if in the configuration there are multiple servers with the same port.
	In such cases, no new Server is created, but they are merge together, to
	emulate the virtual server.
*/
bool Poll::isPortAlreadyInUse( std::map<std::string, std::string> serverConf ) {
	std::vector<Server>::iterator it = _serverList.begin();
	while (it != _serverList.end()) {
		if ((*it).getPort() == serverConf["port"])
			return (true);
		it++;
	}
	return (false);
}

void Poll::mergeServer( Server& s, std::map<std::string, std::string> serverConf ) {
	s.addServerName(serverConf["server_name"]);
	s.addRoot(serverConf["root"]);
	s.addIndex(serverConf["index"]);
}

void Poll::init( void ) {
	std::vector<Server>::iterator serverIt = _serverList.begin();

	while (serverIt != _serverList.end()) {
		(*serverIt).setPortAvailable();
		(*serverIt).bindSocket();
		(*serverIt).listenToRequests();
		serverIt++;
	}
}

void Poll::start( void ) {
	struct pollfd fd[_totalMonitored]; // maybe need to create e member in class and not localscope

	while (true)
	{
		try {
			int eventsNum = poll(fd, _totalMonitored, 3000);

			if (eventsNum < 0) {
				Exception pollException("Poll exception", POLL_FAILED);
				throw pollException;
			}

			if (eventsNum == 0)
				std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;

			if (eventsNum > 0)
			{
				std::cout << "handleEvents()" << std::endl;;
			}
		} catch(Exception const &exception) {
			throw exception;
		}
	}


	// for (unsigned int i = 0; i < _monitoredFdsNum; i++)	{
	// 	handleListeningSocket();
	// 	handleConnectingSocket();
	// }
	return;
}

size_t Poll::setTotalMonitored( void ) {
	std::vector<Server>::iterator it = _serverList.begin();
	size_t counter = 0;
	while (it != _serverList.end()) {
		counter += (*it).getMonitoredFdsNum();
		it++;
	}
}
