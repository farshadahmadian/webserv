/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/22 12:02:48 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include "Poll.hpp"

bool stopServer = false;

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
	Server::logMessage("INFO: The Poll Constructor Called!");
	createServers(configuration);
	initFds();
}
Poll::~Poll(void)
{
	delete[] _totalFds;
	this->_totalFds = NULL;
	
};

/*	By iterating through the instances which come from the configuration file,
	the server are created. If multiple server have the same port, instad of being
	created, they are merged together, emulating the so called virtual server.
*/
void Poll::createServers(const Parser &configuration)
{
	Server::logMessage("INFO: The Servers In The Poll Constructor Created!");
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
	Server::logMessage("INFO: The Poll On The Server List start To set!");
	std::vector<Server>::iterator serverIt = _serverList.begin();

	while (serverIt != _serverList.end())
	{
		(*serverIt).setPortAvailable();
		(*serverIt).bindSocket();
		(*serverIt).listenToRequests();
		serverIt++;
	}
}

void signalHandler(int sigNum) {
	std::cerr << "signal: " << sigNum << "received" << std::endl;
	if (sigNum == SIGINT)
		stopServer = true;
}

void Poll::start(void)
{
	int counter = 0;
	while (true)
	{
		counter++;
		// std::cout << BLUE << "* Poll counter = " << counter <<  RESET << std::endl;

		signal(SIGPIPE, SIG_IGN);
		// signal(SIGPIPE, signalHandler);
		// signal(SIGINT, SIG_IGN);
		signal(SIGINT, signalHandler);
		// signal(SIGABRT, SIG_IGN);
		try
		{
			// printCurrentPollFdsTEST(_currentMonitored, _totalFds);
			int eventsNum = poll(_totalFds, _currentMonitored, 10000);
			cleanConnectedSockets(counter);
			// printCurrentPollFdsTEST(_currentMonitored, _totalFds);
			// std::cout << "* Event num: " << eventsNum << std::endl; 
			// this->printCurrentPollFds();
			if (eventsNum < 0)
			{
				Server::logMessage("WARNING: EventsNum Is Less Than: 0");
				Exception pollException("Poll exception", POLL_FAILED);
				throw pollException;
			}
			if (eventsNum == 0)
			{
				Server::logMessage("WARNING: EventsNum Is Equal To: 0");
				// std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;
			}
			if (eventsNum > 0)
			{
				handleEvent(counter);
			}

			if (stopServer)
				break;
	
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
			handleConnectedEvent(connectedSocketIt->second.getSocketFd(), (*serverIt), &connectedSocketIt);
			i++;
			if (connectedSocketIt == connectedSocketItEnd)
				break;
		}
		// this->removeClosedSocketsFromMap(*serverIt);
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

			int connectedSocketFd = s.acceptFirstRequestInQueue(true);
			addConnectedSocketToMonitoredList(connectedSocketFd);
			s.getConnectedSockets()[connectedSocketFd].setIterationNum(counter);
			this->_totalFds[i].revents = 0;
			// this->printCurrentPollFds();
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
}

void Poll::handleConnectedEvent(int connectedSocketFd, Server &s, std::map<int, ConnectedSocket>::iterator *connectedSocketIt)
{
	try
	{
		nfds_t i = this->mapConnectedSocketFdToPollFd(connectedSocketFd);
		if ((_totalFds[i].revents & POLLIN)) {
			if (!this->receiveRequest(s, i, connectedSocketFd, connectedSocketIt))
				return;
		}
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP))
		{
			int closeResult = 0;
			if (this->_totalFds[i].fd >= 0)
				closeResult = close(this->_totalFds[i].fd);
		
			if (closeResult == -1)
			{
				std::cout << RED << "POLL ERROR - CLOSING FAILED! fd: " << this->_totalFds[i].fd << RESET << std::endl;
				std::cout << errno << std::endl;
			}
		}
		if (((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL)) && (this->_totalFds[i].fd != -1)) {
			s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
			// this->removeClosedSocketsFromMap(s);
			std::map<int, ConnectedSocket>::iterator temp = *connectedSocketIt;
			temp++;
			s.getConnectedSockets().erase(*connectedSocketIt);
			*connectedSocketIt = temp;
			this->_totalFds[i].fd = -1;
			this->_totalFds[i].revents = 0;
			this->removeClosedSocketsFromPollFds();
			return;
		}
		else if (_totalFds[i].revents & POLLOUT)
			this->sendResponse(s, i, connectedSocketFd, connectedSocketIt);
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
	if (_currentMonitored > _totalMonitored) {
		Server::logMessage("ERROR: CurrentMonitored Fd Is Bigger Than The _totalMonitored!");
		std::cout << "??????????????\n";
		exit(1);
	}
}

void Poll::initFds(void)
{
	Server::logMessage("INFO: The Fds In The Poll Constructor Initiated!");
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
			Server::logMessage("ERROR: The fcntl F_SETFL Error Set!");
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
				// std::cout << "------------- 0/1/2 -------------" << std::endl;
			return i;
		}
	}
	// std::cout << "++++++++++++++++++++ 0 +++++++++++++++++++++ " << std::endl;
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
	Server::logMessage("INFO: Connected Sockets Cleaned!");
	std::vector<Server>::iterator serverIt;
	std::vector<Server>::iterator serverItEnd = this->_serverList.end();

	for (serverIt = this->_serverList.begin(); serverIt != serverItEnd; serverIt++)
	{
			std::map<int, ConnectedSocket>::iterator connectedSocketIt = serverIt->getConnectedSockets().begin();
			std::map<int, ConnectedSocket>::iterator connectedSocketItEnd = serverIt->getConnectedSockets().end();

			while (connectedSocketIt != connectedSocketItEnd)
			{
				nfds_t pollNum = this->mapConnectedSocketFdToPollFd(connectedSocketIt->second.getSocketFd());
				if (pollNum < this->_serverList.size() )
				{
					connectedSocketIt->second.setIsConnected(false);
					std::map<int, ConnectedSocket>::iterator temp = connectedSocketIt;
					temp++;
					serverIt->getConnectedSockets().erase(connectedSocketIt);
					connectedSocketIt = temp;
					continue;
				}

				time_t now = time(NULL);
				if (serverIt->getKeepAliveTimeout() && (serverIt->getKeepAliveTimeout() + connectedSocketIt->second.getConnectionStartTime() < now ) && !(this->_totalFds[pollNum].revents & POLLIN) && !(this->_totalFds[pollNum].revents & POLLOUT)) {
					closeTimedoutSockets(pollNum, connectedSocketIt->second);
				}
				else if (!(serverIt->getKeepAliveTimeout()) && (counter > connectedSocketIt->second.getIterationNum() + 8) && !(this->_totalFds[pollNum].revents & POLLIN) && !(this->_totalFds[pollNum].revents & POLLOUT)) {
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
		int connectedSocketFd = s.acceptFirstRequestInQueue(false);
		// s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
		// this->removeClosedSocketsFromMap(s);
		this->_totalFds[i].revents = 0;
		std::cout << "Server " << this->_totalFds[i].fd << " has reached the max " << MAX_CONNECTIONS << " capacity!" << std::endl;
		if (connectedSocketFd >= 0)
			close(connectedSocketFd);
		return true;
	}
	return false;
}

bool Poll::receiveRequest(Server &s, size_t i, int connectedSocketFd, std::map<int, ConnectedSocket>::iterator *connectedSocketIt) {
	(void)connectedSocketFd;
	// std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLIN happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
	// std::cout << "RECEIVING THE REQUEST..." << std::endl;
	if (s.getHttpReq().handleRequest(this->_totalFds[i].fd, this->_totalFds, i, s.getConnectedSockets()[connectedSocketFd]))
	{
		if (!(this->_totalFds[i].revents & POLLHUP) && (*connectedSocketIt)->second.getState() == DONE) {
			// this->_totalFds[i].events = POLLOUT;// | POLLIN;
			s.getHttpResp().handleResponse(this->_totalFds[i].fd, POLLIN_TMP, this->_totalFds, i, s.getConnectedSockets()[connectedSocketFd]);
		}
		return (true);
	} else {
		int closeResult = 0;
		if (this->_totalFds[i].fd >= 0)
			closeResult = close(this->_totalFds[i].fd);
		if (closeResult == -1)
		{
			std::cout << RED << "CLOSING FAILED! ==> fd:" << this->_totalFds[i].fd << RESET << std::endl;
		}
		s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
		// this->removeClosedSocketsFromMap(s);
		std::map<int, ConnectedSocket>::iterator temp = *connectedSocketIt;
		temp++;
		s.getConnectedSockets().erase(*connectedSocketIt);
		*connectedSocketIt = temp;
		this->_totalFds[i].fd = -1;
		this->_totalFds[i].revents = 0;
		this->removeClosedSocketsFromPollFds();
	}
	return (false);
}

void Poll::sendResponse(Server &s, size_t i, int connectedSocketFd, std::map<int, ConnectedSocket>::iterator *connectedSocketIt) {
	// std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLOUT happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;
	if(!(s.getHttpResp().handleResponse(this->_totalFds[i].fd, POLLOUT_TMP, this->_totalFds, i, s.getConnectedSockets()[connectedSocketFd]))) {
		int closeResult = 0;
		if (this->_totalFds[i].fd >= 0)
			closeResult = close(this->_totalFds[i].fd);
		if (closeResult == -1)
		{
			std::cout << RED << "CLOSING FAILED! ==> fd:" << this->_totalFds[i].fd << RESET << std::endl;
		}
		s.getConnectedSockets()[connectedSocketFd].setIsConnected(false);
		// this->removeClosedSocketsFromMap(s);
		std::map<int, ConnectedSocket>::iterator temp = *connectedSocketIt;
		temp++;
		s.getConnectedSockets().erase(*connectedSocketIt);
		*connectedSocketIt = temp;
		this->_totalFds[i].fd = -1;
		this->_totalFds[i].revents = 0;
		this->removeClosedSocketsFromPollFds();
		return;
	}
	
	time_t now = time(NULL);
	s.getConnectedSockets()[connectedSocketFd].clearRequestProperties();
	if (s.getKeepAliveTimeout() && (now < s.getConnectedSockets()[connectedSocketFd].getConnectionStartTime() + s.getKeepAliveTimeout())) {
		// this->_totalFds[i].events = POLLIN;
		// this->_totalFds[i].revents = 0;
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
	std::map<int, ConnectedSocket>::iterator temp = *connectedSocketIt;
	temp++;
	s.getConnectedSockets().erase(*connectedSocketIt);
	*connectedSocketIt = temp;
	this->_totalFds[i].fd = -1;
	this->removeClosedSocketsFromPollFds();
}
