/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/06 09:18:30 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

void printCurrentPollFdsTEST(nfds_t currentMonitored, struct pollfd* pollFd) {
	nfds_t i = 0;
	std::cout << BLUE << "CURRENT PollFds List: " << RESET << std::endl;
	while (i < currentMonitored) {
		std::cout << "totalFds[" << i << "].fd = " << pollFd[i].fd << std::endl;
		std::cout << "totalFds[" << i << "].events = " << std::hex << pollFd[i].events << std::dec << std::endl;
		std::cout << "totalFds[" << i << "].revents = " << std::hex << pollFd[i].revents << std::dec << std::endl;
		i++;
	}
	return;
}

Poll::Poll(const Parser &configuration) : _serverList(std::vector<Server>()),
	_currentMonitored(0),
	_totalMonitored(0),
	_totalFds(NULL)
{
	createServers(configuration);
	initFds();
}
Poll::~Poll(void)
{
	std::cout << BLUE << "Poll destructor called" RESET << std::endl;
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
			{
				serverConfIt++;
				continue;
			}
		}
		Server s((*serverConfIt).getSettings());
		_serverList.push_back(s);
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
			// (*it).setKeepAliveTimeout(serverConf["keepalive_timeout"]);
			// (*it).addKeepAlive(serverConf[""]);
			// (*it).addAutoindex(serverConf["index"]);
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
		// std::cout << BLUE << "* Poll counter = " << counter <<  RESET << std::endl;
		try
		{
			int eventsNum = poll(_totalFds, _currentMonitored, 3000);
			cleanConnectedSockets(counter);
			// std::cout << "* Event num: " << eventsNum << std::endl; 
			// this->printCurrentPollFds();
			if (eventsNum < 0)
			{
				Exception pollException("Poll exception", POLL_FAILED);
				throw pollException;
			}
			if (eventsNum == 0)
			{
				// std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;
			}
			if (eventsNum > 0)
			{
				handleEvent(counter);
			}
		printCurrentPollFdsTEST(_currentMonitored, _totalFds);
		}
		catch (Exception const &exception)
		{
			throw exception;
		}
	}
}

void Poll::handleEvent(int counter)
{
	std::vector<Server>::iterator serverIt = _serverList.begin();

	size_t i = 0;
	while (serverIt != _serverList.end())
	{
		// std::cout << serverIt->getListeningSocket().getSocketFd() << ": " << _totalFds[i].fd << std::endl;
		if (_totalFds[i].fd == (*serverIt).getListeningSocket().getSocketFd())
		{
			handleListeningEvent(i, (*serverIt), counter);
			serverIt++;
			i++;
			//  std::cout << RED << "Iteration count on servers: " << i << RESET << std::endl; 
		}
		else {
			serverIt++;
			i++;
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
			handleConnectedEvent(connectedSocketIt->second.getSocketFd(), (*serverIt));
			i++;
		}
		this->removeClosedSocketsFromMap(*serverIt);
		// serverIt->printConnectedSockets();
		// this->printCurrentPollFds();
		// this->printAllPollFds();
		serverIt++;
	}
}

void Poll::handleListeningEvent(size_t i, Server &s, int counter)
{
	try
	{
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL))
		{
			std::cout << PURPLE << "************** close fd, remove from pollfds list ***************" << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[" << i << "].revents & POLLERR = " << (this->_totalFds[i].revents & POLLERR) << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[" << i << "].revents & POLLHUP = " <<(this->_totalFds[i].revents & POLLHUP) << RESET << std::endl;
			std::cout << PURPLE << "this->_totalFds[" << i << "].revents & POLLNVAL = " << (this->_totalFds[i].revents & POLLNVAL) << RESET << std::endl;
			// exit(10);
		}
		if ((_totalFds[i].revents & POLLIN))
		{
			// std::cout << GREEN << "Port [" << s.getPort() << "] " << " * event happened on listeningSocket: " << _totalFds[i].fd << RESET << std::endl;
			if (this->isMaxConnection(s, i))
				return;

			int connectedSocketFd = s.acceptFirstRequestInQueue();
			addConnectedSocketToMonitoredList(connectedSocketFd);
			s.getConnectedSockets()[connectedSocketFd]._iterationNum = counter;
			this->_totalFds[i].revents = 0;
			// this->printCurrentPollFds();
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
}

void Poll::handleConnectedEvent(int connectedSocketFd, Server &s)
{
	try
	{
		nfds_t i = this->mapConnectedSocketFdToPollFd(connectedSocketFd);
		if ((_totalFds[i].revents & POLLIN))
			this->receiveRequest(s, i);
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP))
		{
			int closeResult = 0;
			if (this->_totalFds[i].fd >= 0)
				closeResult = close(this->_totalFds[i].fd);
		
			if (closeResult == -1)
			{
				std::cout << RED << "POLL ERROR - CLOSING FAILED! fd: " << this->_totalFds[i].fd << RESET << std::endl;
				strerror(errno);
			}
		}
		else if (_totalFds[i].revents & POLLOUT)
			this->sendResponse(s, i, connectedSocketFd);
		if (((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL)) && (this->_totalFds[i].fd != -1)) {
			s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
			// this->removeClosedSocketsFromMap(s);
			this->_totalFds[i].fd = -1;
			this->removeClosedSocketsFromPollFds();
			return;
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
	if (this->_currentMonitored >= this->_totalMonitored)
		return;
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

		if (fcntl(_totalFds[i].fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
		}
		it++;
		i++;
	}
}

void Poll::removeClosedSocketsFromMap(Server &s) {
	if (!(s.getConnectedSockets().size()))
		return;
	std::map<int, ConnectedSocket>::iterator it = s.getConnectedSockets().begin();
	while (it != s.getConnectedSockets().end()) {
		if (it->second.getIsConnected() == false) {
			std::map<int, ConnectedSocket>::iterator temp = it;
			temp++;
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
	// std::ofstream stream("test.txt", std::ios::app);
	std::cout << BLUE << "CURRENT PollFds List: " << RESET << std::endl;
	while (i < this->_currentMonitored) {
		std::cout << "totalFds[" << i << "].fd = " << this->_totalFds[i].fd << std::endl;
		std::cout << "totalFds[" << i << "].events = " << std::hex << this->_totalFds[i].events << std::dec << std::endl;
		std::cout << "totalFds[" << i << "].revents = " << std::hex << this->_totalFds[i].revents << std::dec << std::endl;
		// std::cout << "totalFds[" << i << "].fd = " << this->_totalFds[i].fd << std::endl;
		// std::cout << "totalFds[" << i << "].events = " << this->_totalFds[i].events << std::endl;
		// std::cout << "totalFds[" << i << "].revents = " << this->_totalFds[i].revents << std::endl;
		i++;
	}
	// stream.close();
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

nfds_t Poll::mapConnectedSocketFdToPollFd(int connectedSocketFd) {
	for (nfds_t i = 0; i < this->_currentMonitored; i++)
	{
		if (this->_totalFds[i].fd == connectedSocketFd)
		{
			// std::cout << this->_totalFds[i].fd << " = " << connectedSocketFd << std::endl;
			// if (i == 0 || i == 1 || i == 2)
			// 	std::cout << "!" << std::endl;
			return i;
		}
	}
	// std::cout << "?" << std::endl;
	return 0;
}

void Poll::closeTimedoutSockets(nfds_t pollNum, ConnectedSocket &connectedSocket) {
	if (this->_totalFds[pollNum].fd > 0) {
			close(this->_totalFds[pollNum].fd);
		}
		connectedSocket.setIsConnected(false);
		this->_totalFds[pollNum].fd = -1;
}

void Poll::cleanConnectedSockets(int counter) {
	std::vector<Server>::iterator serverIt;
	std::vector<Server>::iterator serverItEnd = this->_serverList.end();

	for (serverIt = this->_serverList.begin(); serverIt != serverItEnd ; serverIt++)
	{
			std::map<int, ConnectedSocket>::iterator connectedSocketIt = serverIt->getConnectedSockets().begin();
			std::map<int, ConnectedSocket>::iterator connectedSocketItEnd = serverIt->getConnectedSockets().end();

			while (connectedSocketIt != connectedSocketItEnd)
			{
								nfds_t pollNum = this->mapConnectedSocketFdToPollFd(connectedSocketIt->second.getSocketFd());
				if (pollNum < this->_serverList.size() )
				{
					connectedSocketIt->second.setIsConnected(false);
					this->removeClosedSocketsFromMap(*serverIt);
					connectedSocketIt++;
					continue;
				}

				time_t now = time(NULL);
				if (serverIt->getKeepAliveTimeout() && (serverIt->getKeepAliveTimeout() + connectedSocketIt->second.getConnectionStartTime() < now ) && !(this->_totalFds[pollNum].revents & POLLIN) && !(this->_totalFds[pollNum].revents & POLLOUT)) {
					closeTimedoutSockets(pollNum, connectedSocketIt->second);
				}
				else if (!(serverIt->getKeepAliveTimeout()) && (counter > connectedSocketIt->second._iterationNum + 8) && !(this->_totalFds[pollNum].revents & POLLIN) && !(this->_totalFds[pollNum].revents & POLLOUT)) {
					closeTimedoutSockets(pollNum, connectedSocketIt->second);
				}

				connectedSocketIt++;
			}
			this->removeClosedSocketsFromMap(*serverIt);
			this->removeClosedSocketsFromPollFds();
	}
}

bool Poll::isMaxConnection(Server &s, size_t i) {
	if (s.getConnectedSockets().size() >= MAX_CONNECTIONS) {
		int connectedSocketFd = s.acceptFirstRequestInQueue();
		s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
		this->removeClosedSocketsFromMap(s);
		this->_totalFds[i].revents = 0;
		std::cout << "Server " << this->_totalFds[i].fd << " has reached the max " << MAX_CONNECTIONS << " capacity!" << std::endl;
		if (connectedSocketFd >= 0)
			close(connectedSocketFd);
		return true;
	}
	return false;
}

void Poll::receiveRequest(Server &s, size_t i) {
	// std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLIN happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
	// std::cout << "RECEIVING THE REQUEST..." << std::endl;
	if (s.getHttpReq().handleRequest(this->_totalFds[i].fd))
	{
		if (!(this->_totalFds[i].revents & POLLHUP)) {
			this->_totalFds[i].events = POLLOUT;
			s.getHttpResp().handleRespons(this->_totalFds[i].fd, POLLIN_TMP);
		}
	}
}

void Poll::sendResponse(Server &s, size_t i, int connectedSocketFd) {
	std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLOUT happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
	s.getHttpResp().handleRespons(this->_totalFds[i].fd, POLLOUT_TMP);//navid_code
	
	time_t now = time(NULL);

	if (s.getKeepAliveTimeout() && (now < s.getConnectedSockets()[connectedSocketFd].getConnectionStartTime() + s.getKeepAliveTimeout())) {
		this->_totalFds[i].events = POLLIN;
		this->_totalFds[i].revents = 0;
		return;
	}


	int closeResult = 0;
	if (this->_totalFds[i].fd >= 0)
		closeResult = close(this->_totalFds[i].fd);
	if (closeResult == -1)
	{
		std::cout << RED << "CLOSING FAILED! ==> fd:" << this->_totalFds[i].fd << RESET << std::endl;
	}
	s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
	// this->removeClosedSocketsFromMap(s);
	this->_totalFds[i].fd = -1;
	this->removeClosedSocketsFromPollFds();
}
