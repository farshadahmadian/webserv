/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/05 11:11:47 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll(const Parser &configuration) : _serverList(std::vector<Server>()),
	_currentMonitored(0),
	_totalMonitored(0),
	_totalFds(NULL)
{
	// std::cout << BLUE << "Poll constructor called" RESET << std::endl;
	createServers(configuration);
	initFds();
	/* REMOVE */ std::cout << std::endl
	<< "---------------------------------------------------------" << std::endl;
}
Poll::~Poll(void)
{
	// std::cout << BLUE << "Poll destructor called" RESET << std::endl;
	delete[] _totalFds;
	this->_totalFds = NULL;
};

/*	By iterating through the instances which come from the configuration file,
	the server are created. If multiple server have the same port, instad of being
	created, they are merged together, emulating the so called virtual server.
*/
void Poll::createServers(const Parser &configuration)
{
	std::vector<ServerConf>::const_iterator serverConfIt = configuration.getHttp().getServer().begin();
	while (serverConfIt != configuration.getHttp().getServer().end())
	{
		if (_serverList.size() > 0)
		{
			std::map<std::string, std::string> currentServerSettings = (*serverConfIt).getSettings();
			if (mergeServerWithSamePort(currentServerSettings))
			{ // check if the port is already in use
				serverConfIt++;
				continue;
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
bool Poll::mergeServerWithSamePort(std::map<std::string, std::string> serverConf)
{
	std::vector<Server>::iterator it = _serverList.begin();
	while (it != _serverList.end())
	{
		if ((*it).getPort() == serverConf["port"])
		{
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

void Poll::init(void)
{
	std::vector<Server>::iterator serverIt = _serverList.begin();

	while (serverIt != _serverList.end())
	{
		(*serverIt).setPortAvailable();
		(*serverIt).bindSocket();
		(*serverIt).listenToRequests();
		serverIt++;
	}
}

void Poll::start(void)
{
	int counter = 0;

	while (true)
	{
		counter++;
		std::cout << BLUE << "* Poll counter = " << counter <<  RESET << std::endl;
		try
		{
			int eventsNum = poll(_totalFds, _currentMonitored, 3000);
			std::cout << "* Event num: " << eventsNum << std::endl;
			// this->printCurrentPollFds();
			if (eventsNum < 0)
			{
				Exception pollException("Poll exception", POLL_FAILED);
				throw pollException;
			}
			if (eventsNum == 0)
			{
				std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;
			}
			if (eventsNum > 0)
			{
				handleEvent();
			}
		}
		catch (Exception const &exception)
		{
			throw exception;
		}
	}
}

void Poll::handleEvent(void)
{
	std::vector<Server>::iterator serverIt = _serverList.begin();

	size_t i = 0;
	while (serverIt != _serverList.end())
	{

		if (_totalFds[i].fd == (*serverIt).getListeningSocket().getSocketFd())
		{
			handleListeningEvent(i, (*serverIt));
			serverIt++;
			i++;
			//  std::cout << RED << "Iteration count on servers: " << i << RESET << std::endl;
		}
	}
	serverIt = this->_serverList.begin();
	while (serverIt != _serverList.end())
	{
		i = 0;
		std::map<int, ConnectedSocket>::iterator connectedSocketIt;
		std::map<int, ConnectedSocket>::iterator connectedSocketItEnd = serverIt->getConnectedSockets().end();

		for (connectedSocketIt = serverIt->getConnectedSockets().begin(); connectedSocketIt != connectedSocketItEnd; connectedSocketIt++)
		{
			handleConnectedEvent(i + this->_serverList.size(), (*serverIt));
			i++;
		}
		removeClosedSocketsFromMap(*serverIt);
		serverIt->printConnectedSockets();
		// // this->printCurrentPollFds();
		// this->printAllPollFds();
		serverIt++;
	}
}

void Poll::handleListeningEvent(size_t i, Server &s)
{
	try
	{
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL))
		{
			std::cout << PURPLE << "************** close fd, remove from pollfds list ***************" << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[i].revents & POLLERR = " << (this->_totalFds[i].revents & POLLERR) << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[i].revents & POLLHUP = " <<(this->_totalFds[i].revents & POLLHUP) << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[i].revents & POLLNVAL = " << (this->_totalFds[i].revents & POLLNVAL) << RESET << std::endl;
			exit(10);
		}
		if ((_totalFds[i].revents & POLLIN))
		{ // && (this->_monitoredFdsNum <= MAX_CONNECTIONS))	{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * event happened on listeningSocket: " << _totalFds[i].fd << RESET << std::endl;
			int connectedSocketFd = s.acceptFirstRequestInQueue();

			addConnectedSocketToMonitoredList(connectedSocketFd);
			this->_totalFds[i].revents = 0;
			// this->printCurrentPollFds();
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
}

void Poll::handleConnectedEvent(size_t i, Server &s)
{
	try
	{
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP))
		{
			std::cout << PURPLE << "**************################## close fd, remove from pollfds list, remove from connectedSockets list **************##################" << RESET << std::endl;
			std::cout << PURPLE << "**************################## this->_totalFds[i].revents & POLLERR = " << (this->_totalFds[i].revents & POLLERR) << " **************##################" << RESET << std::endl;
			std::cout << PURPLE << "**************################## this->_totalFds[i].revents & POLLHUP = " << (this->_totalFds[i].revents & POLLHUP) << " **************##################" << RESET << std::endl;
			std::cout << PURPLE << "**************################## this->_totalFds[i].revents & POLLNVAL = " << (this->_totalFds[i].revents & POLLNVAL) << " **************##################" << RESET << std::endl;
			int closeResult = close(this->_totalFds[i].fd);

			if (closeResult == -1)
			{
				std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
			}
		}

		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL)) {
			s.getConnectedSockets()[this->_totalFds[i].fd].setIsConnected(false);
			this->_totalFds[i].fd = -1;
			// s.closeSocket();
			this->removeClosedSocketsFromPollFds();
			return ;
		}

		if ((_totalFds[i].revents & POLLIN))
		{ // && (this->_monitoredFdsNum <= MAX_CONNECTIONS))	{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLIN happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;

			std::cout << "RECEIVING THE REQUEST..." << std::endl;
			// naivd_code from here ->
			// HTTPRequest httpreq(s.getSettings()); // navid_code
			if (s.getHttpReq().handleRequest(this->_totalFds[i].fd))
			{
				// s.initializeResponse();//navid_code
				this->_totalFds[i].events = POLLIN | POLLOUT;
				s.getHttpResp().handleResponse(this->_totalFds[i].fd, POLLIN_TMP); //navid_code
			}
		}
		if (_totalFds[i].revents & POLLOUT)
		{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLOUT happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
				// s.initializeResponse();//navid_code
				s.getHttpResp().handleResponse(this->_totalFds[i].fd, POLLOUT_TMP);//navid_code
				int closeResult = close(this->_totalFds[i].fd);
				if (closeResult == -1)
				{
					std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
				}
				std::cout << RED << "Socket [" << this->_totalFds[i].fd << "] is closed." << RESET << std::endl;
				s.getConnectedSockets()[this->_totalFds[i].fd].setIsConnected(false);
				// s.getResponses().erase(this->_totalFds[i].fd);
				this->_totalFds[i].fd = -1;
				// s.closeSocket();
				this->removeClosedSocketsFromPollFds();
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
}

void Poll::addConnectedSocketToMonitoredList(int connectedSocketFd)
{
	for (size_t i = 0; i < _totalMonitored; i++)
	{
		if (_totalFds[i].fd == -1)
		{
			_totalFds[i].fd = connectedSocketFd;
			// _totalFds[i].revents = 0;
			// _totalFds[i].events = POLLIN | POLLOUT;
			_totalFds[i].events = POLLIN;
			break;
		}
	}
	_currentMonitored++;
}

void Poll::initFds(void)
{
	size_t n_servers = _serverList.size();
	_totalMonitored = (n_servers * MAX_CONNECTIONS) + n_servers;
	_currentMonitored = static_cast<nfds_t>(n_servers);

	_totalFds = new struct pollfd[_totalMonitored];
	memset(this->_totalFds, 0, sizeof(struct pollfd) * this->_totalMonitored);
	std::vector<Server>::iterator it = _serverList.begin();
	size_t i = 0;

	while (i < this->_totalMonitored)
	{
		this->_totalFds[i].fd = -1;
		i++;
	}

	i = 0;
	while (it != _serverList.end())
	{
		_totalFds[i].fd = (*it).getListeningSocket().getSocketFd();
		_totalFds[i].events = POLLIN;

	// 	if (fcntl(_totalFds[i].fd, F_SETFL, O_NONBLOCK) == -1) {
  //       perror("fcntl F_SETFL");
	// }

		it++;
		i++;
	}
}

void Poll::removeClosedSocketsFromMap(Server &s) {
	std::map<int, ConnectedSocket>::iterator it = s.getConnectedSockets().begin();
	while (it != s.getConnectedSockets().end()) {
		if (it->second.getIsConnected() == false) {
			std::map<int, ConnectedSocket>::iterator temp = it;
			temp++;
			std::cout << "connectedSocket[" << it->second.getSocketFd() << "] is removed from server " << s.getListeningSocket().getSocketFd() << std::endl;
			s.getConnectedSockets().erase(it);
			it = temp;
		}
		else {
			it++;
		}
	}
}

void Poll::removeClosedSocketsFromPollFds(void) {
	for (nfds_t i = 0; i < this->_currentMonitored;)
		{
			if (this->_totalFds[i].fd == -1)
			{
				this->_totalFds[i].events = 0;
				this->_totalFds[i].revents = 0;
				for (nfds_t j = i; j < this->_currentMonitored - 1; ++j)
				{
					this->_totalFds[j].fd = this->_totalFds[j + 1].fd;
					this->_totalFds[j].events = this->_totalFds[j + 1].events;
					this->_totalFds[j].revents = this->_totalFds[j + 1].revents;
					this->_totalFds[j + 1].fd = -1;
					this->_totalFds[j + 1].events = 0;
					this->_totalFds[j + 1].revents = 0;
				}
				this->_currentMonitored--;
			}
			else
			{
				i++;
			}
		}
	return;
}

void Poll::printCurrentPollFds(void) {
	nfds_t i = 0;
	std::cout << BLUE << "CURRENT PollFds List: " << RESET << std::endl;
	while (i < this->_currentMonitored) {
		std::cout << "totalFds[" << i << "].fd = " << this->_totalFds[i].fd << std::endl;
		std::cout << "totalFds[" << i << "].events = " << this->_totalFds[i].events << std::endl;
		std::cout << "totalFds[" << i << "].revents = " << this->_totalFds[i].revents << std::endl;
		i++;
	}
	return;
}

void Poll::printAllPollFds(void) {
	nfds_t i = 0;
	std::cout << BLUE << "ALL PollFds List: " << RESET << std::endl;
	while (i < this->_totalMonitored) {
		std::cout << "totalFds[" << i << "].fd = " << this->_totalFds[i].fd << std::endl;
		std::cout << "totalFds[" << i << "].events = " << this->_totalFds[i].events << std::endl;
		std::cout << "totalFds[" << i << "].revents = " << this->_totalFds[i].revents << std::endl;
		i++;
	}
	return;
}
