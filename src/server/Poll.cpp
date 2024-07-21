/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/21 14:28:35 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll( const Parser& configuration ):
	_serverList(std::vector<Server>()),
	_totalMonitored(0) {

	createServers(configuration);
	setFds();
	setTotalMonitored();
/* REMOVE */std::cout << std::endl << "---------------------------------------------------------" << std::endl;
}
Poll::~Poll( void ) {};

/*	By iterating through the instances which come from the configuration file,
	the server are created. If multiple server have the same port, instad of being
	created, they are merged together, emulating the so called virtual server.
*/
void Poll::createServers( const Parser& configuration ) {
	std::vector<ServerConf>::const_iterator serverConfIt = configuration.getHttp().getServer().begin();
	while (serverConfIt != configuration.getHttp().getServer().end()) {
		if (_serverList.size() > 0) {
			std::map<std::string, std::string> currentServerSettings = (*serverConfIt).getSettings();
			if (mergeServerWithSamePort(currentServerSettings)) { // check if the port is already in use
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

/*	Check if in the configuration there are multiple servers with the same port.
	In such cases, no new Server is created, but they are merge together, to
	emulate the virtual server.
*/
bool Poll::mergeServerWithSamePort( std::map<std::string, std::string> serverConf ) {
	std::vector<Server>::iterator it = _serverList.begin();
	while (it != _serverList.end()) {
		if ((*it).getPort() == serverConf["port"]) {
			(*it).addServerName(serverConf["server_name"]);
			(*it).addRoot(serverConf["root"]);
			(*it).addIndex(serverConf["index"]);
			return (true);
		}
		it++;
	}
	return (false);
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

/*	For each Server, a struct poll fd is locally created, and the listening fd
	of the current iterated Server, assigned to it.
*/
void Poll::setFds( void ) {
	std::vector<Server>::iterator it = _serverList.begin();
	while (it != _serverList.end()) {
		struct pollfd newPoll;
		newPoll.fd = (*it).getListeningSocket().getSocketFd();
		newPoll.events = POLLIN;
		_fds.push_back(newPoll);
		it++;
	}
}

void Poll::start( void ) {
	std::cout << BLUE << "* Poll::start()" RESET << std::endl;

	/*
		prepare totalFds with all fds of all servers, and copy them here
		struct pollfd totalFds[_totalMonitored]; ----> correct approach? is it ok to have 50 fds in
		totalFdss, while only 3 of them are the listening?
	*/
	struct pollfd totalFds[_totalMonitored]; // ------ ??????


	while (true)
	{
		try {
			int eventsNum = poll(totalFds, _totalMonitored, 3000);
			std::cout << "* Event num: " << eventsNum << std::endl;

			if (eventsNum < 0) {
				Exception pollException("Poll exception", POLL_FAILED);
				throw pollException;
			}
			if (eventsNum == 0) {
				std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;
			}
			if (eventsNum > 0) {
				std::cout << "* handleEvents()" << std::endl;
				// handleEvents();
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

void Poll::handleEvent( void ) {
	// std::vector<Server>::iterator it = _serverList.begin();

	// while (it != _serverList.end()) {

	// 	it++;
	// }
}

void Poll::setTotalMonitored( void ) {
	std::vector<Server>::iterator it = _serverList.begin();
	while (it != _serverList.end()) {
		_totalMonitored += (*it).getMonitoredFdsNum();
		it++;
	}
	std::cout << BLUE << "* Poll::setTotalMonitored(): " << RESET << _totalMonitored << std::endl;
}
