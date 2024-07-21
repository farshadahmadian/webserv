/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/21 16:01:45 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll( const Parser& configuration ):
	_serverList(std::vector<Server>()),
	_currentMonitored(0),
	_totalMonitored(0),
	_totalFds(NULL) {

	createServers(configuration);
	setFds();
/* REMOVE */std::cout << std::endl << "---------------------------------------------------------" << std::endl;
}
Poll::~Poll( void ) {
	delete [] _totalFds;
};

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
			// (*it).addAutoindex(serverConf["index"]);
			// (*it).addKeepAlive(serverConf[""]);
			// (*it).addClientSize(serverConf[""]);
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

void Poll::start( void ) {
	std::cout << BLUE << "* Poll::start()" RESET << std::endl;

	while (true)
	{
		try {
			int eventsNum = poll(_totalFds, _currentMonitored, 3000);
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
				handleEvent();
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
	// std::vector<Server>::iterator serverIt = _serverList.begin();

	// for (size_t i = 0; i < _currentMonitored; i++) {
	// 	while (serverIt != _serverList.end()) {
	// 		if (_totalFds[i].fd == (*serverIt).getListeningSocket().getSocketFd())
	// 			handleListeningEvent(i, (*serverIt));
	// 		else
	// 			handleConnectedEvent(i, (*serverIt));
	// 		serverIt++;
	// 	}
	// }
}

void Poll::handleListeningEvent( size_t i, Server& s ) {
	std::cout << GREEN << "Port [" << s.getPort() << "] " << " * event happened on listeningSocket: " << _totalFds[i].fd << RESET << std::endl;
	try {
		if ((_totalFds[i].revents & POLLIN)) { // && (this->_monitoredFdsNum <= MAX_CONNECTIONS))	{
			int connectedSocketFd = s.acceptFirstRequestInQueue();
			updateFds(connectedSocketFd);
		}
	} catch ( Exception const &exception ) {
		throw exception;
	}
}

void Poll::handleConnectedEvent( size_t i, Server& s ) {
	try {
		if ((_totalFds[i].revents & POLLIN)) { // && (this->_monitoredFdsNum <= MAX_CONNECTIONS))	{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLIN happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
		}
		if (_totalFds[i].revents & POLLOUT) {
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLOUT happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;

		}
	} catch ( Exception const &exception ) {
		throw exception;
	}
}

void Poll::updateFds( int connectedSocketFd ) {
	for (size_t i = 0; i <= _totalMonitored ;i++) {
		if (_totalFds[i].fd == -1) {
			_totalFds[i].fd = connectedSocketFd;
			_totalFds[i].events = POLLIN;// | POLLOUT;
			break ;
		}
	}
	_currentMonitored++;
}

void Poll::setFds( void ) {
	size_t n_servers = _serverList.size();
	_totalMonitored = (n_servers * MAX_CONNECTIONS) + n_servers ;
	_currentMonitored = n_servers;

	_totalFds = new struct pollfd[_totalMonitored];
	std::vector<Server>::iterator it = _serverList.begin();
	size_t i = 0;
	while (it != _serverList.end()) {
		_totalFds[i].fd = (*it).getListeningSocket().getSocketFd();
		_totalFds[i].events = POLLIN;
		it++;
		i++;
	}
}
