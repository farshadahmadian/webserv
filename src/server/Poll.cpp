/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:55:19 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/30 09:06:04 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll(const Parser &configuration) : _serverList(std::vector<Server>()),
	_currentMonitored(0),
	_totalMonitored(0),
	_totalFds(NULL)
{
	createServers(configuration);
	initFds();
	/* REMOVE */ std::cout << std::endl
	<< "---------------------------------------------------------" << std::endl;
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
			this->printCurrentPollFds(); 
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
			{ // 2
				// std::cout << "* handleEvents(): " << std::endl;
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

	// for (size_t i = 0; i < _currentMonitored; i++)
	// {
	size_t i = 0;
	while (serverIt != _serverList.end())
	{

		if (_totalFds[i].fd == (*serverIt).getListeningSocket().getSocketFd())
		{
			handleListeningEvent(i, (*serverIt));
			serverIt++;
			i++;
			 std::cout << RED << "Iteration count on servers: " << i << RESET << std::endl; 
			// break;
		}
		// else
		// 	handleConnectedEvent(i, (*serverIt));
		// serverIt++;
	}
	// }
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
			// std::cout << CYAN << "Iteration count on connected sockets in server fd: " << serverIt->getListeningSocket().getSocketFd() << " ===> " << i << SET << std::endl; 
		}
		removeClosedSocketsFromMap(*serverIt);
		// if (serverIt->getConnectedSockets().size())
		// {
			serverIt->printConnectedSockets();
			this->printCurrentPollFds();
		// }
		serverIt++;
	}
	// else
	// 	handleConnectedEvent(i, (*serverIt));
	// serverIt++;
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
			this->printCurrentPollFds();
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
		if ((this->_totalFds[i].revents & POLLERR) || (this->_totalFds[i].revents & POLLHUP) || (this->_totalFds[i].revents & POLLNVAL))
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
			s.getConnectedSockets()[this->_totalFds[i].fd].setIsConnected(false);
			this->_totalFds[i].fd = -1;
			// s.closeSocket();
			this->removeClosedSocketsFromPollFds();
			return ;
		}



		// std::cout << "WAITING ... " << std::endl;
		// sleep(1);
		// std::cout << "CONTINUING ... " << std::endl;
		// if ((this->_totalFds[i].revents & POLLOUT) && !(this->_totalFds[i].revents & POLLIN)) {
		// 	std::cout << YELLOW << "*************** NO POLLIN *************** " << RESET << std::endl;
		// 	std::ostringstream outputString;
		// 	outputString << "HTTP/1.1 408 Request Timeout\r\nConnection: closed\r\n\r\n";
		// 	send(s.getConnectedSockets()[this->_totalFds[i].fd].getSocketFd(), outputString.str().c_str(), sizeof(outputString.str().size()), 0);

		// 	int closeResult = close(this->_totalFds[i].fd);
		// 	if (closeResult == -1)
		// 	{
		// 		std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
		// 	}
		// 	s.getConnectedSockets()[this->_totalFds[i].fd].setIsConnected(false);
		// 	this->_totalFds[i].fd = -1;
		// 	this->removeClosedSocketsFromPollFds();
		// 	return;
		// }


		
		if ((_totalFds[i].revents & POLLIN))
		{ // && (this->_monitoredFdsNum <= MAX_CONNECTIONS))	{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLIN happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;

			std::cout << "RECEIVING THE REQUEST..." << std::endl;
			// naivd_code from here ->
			// HTTPRequest httpreq(s.getSettings()); // navid_code
			if (s.getHttpReq().handleRequest(this->_totalFds[i].fd))
			{
				s.getHttpReq().handleRespons(this->_totalFds[i].fd, POLLIN_TMP); //

				// _POLLINCheck[this->_totalFds[i].fd] = 1;
				// char receive[20048];
				// receive[20047] = '\0';
				// ssize_t result = recv(this->_monitoredFds[i].fd, receive, sizeof(receive) - 1, 0);
				// if (result == -1)
				// 	throw Exception("Receive Failed", RECEIVE_FAILED);
				// // close(this->_monitoredFds[i].fd);
				// // this->_monitoredFdsNum--;
				// this->parseRequest(static_cast<std::string>(receive));
				// this->printRequest();

				// s.getResponses()[this->_totalFds[i].fd] = httpreq.getResponse();
				// std::cout << "Handled request on socket fd " << this->_totalFds[i].fd << std::endl;
				// this->_totalFds[i].events = POLLOUT;
			}
			// else
			// {
			// 	Exception httpRequestException("httpRequest failed!", HTTP_REQUEST_FAILED);
			// 	throw httpRequestException;
			// }
		}
		if (_totalFds[i].revents & POLLOUT)
		{
			std::cout << GREEN << "Port [" << s.getPort() << "] " << " * POLLOUT happened on connectedSocket: " << _totalFds[i].fd << RESET << std::endl;

			// std::string response = s.getResponses()[this->_totalFds[i].fd];
			// //****************print the provided response in file***********************
			//  std::cout << RED "****sending the response\n" RESET; 
			// // writHtmlFile(response, "./src/request/response.txt");
			// std::ofstream outfile("./src/request/response.txt");
			// outfile << response << std::endl;
			// outfile.close();
			// //**************************************************************************

			// send(this->_totalFds[i].fd, response.c_str(), response.size(), 0);
			// int closeResult = close(this->_totalFds[i].fd);
			// if (closeResult == -1)
			// {
			// 	std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
			// }
			// std::cout << RED << "Socket [" << s.getConnectedSockets()[this->_totalFds[i].fd].getSocketFd() << "] is closed." << RESET << d::endl; 
			// if (s.getHttpReq().handleRespons(this->_totalFds[i].fd, POLLOUT_TMP)) {
				s.getHttpReq().handleRespons(this->_totalFds[i].fd, POLLOUT_TMP);
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
				// this->_monitoredFdsNum--;
				// Remove the response from the map
				// return ;
			// } else if (_POLLINCheck[this->_totalFds[i].fd] == 10)
			// {
			// 	std::cout << "pollout check is: " << _POLLINCheck[this->_totalFds[i].fd] << std::endl;
			// 	int closeResult = close(this->_totalFds[i].fd);
			// 	if (closeResult == -1)
			// 	{
			// 		std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
			// 	}
			// 	std::cout << RED << "Socket [" << this->_totalFds[i].fd << "] is closed." << RESET << std::endl;		
			// 	s.getConnectedSockets()[this->_totalFds[i].fd].setIsConnected(false);
			// 	// s.getResponses().erase(this->_totalFds[i].fd);
			// 	this->_totalFds[i].fd = -1;
			// 	// s.closeSocket();
			// 	this->removeClosedSocketsFromPollFds();
			// 	// this->_monitoredFdsNum--;
			// 	// Remove the response from the map
			// 	_POLLINCheck[this->_totalFds[i].fd] = 0;

			// 	return ;
			// }
			// _POLLINCheck[this->_totalFds[i].fd]++;
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
			 std::cout << "******************* BEFORE :" << _totalFds[i].revents << " ****** " << std::endl; 
			// _totalFds[i].revents = 0;
			// std::cout << "******************* AFTER :" << _totalFds[i].revents << " ****** " << std::endl;
			_totalFds[i].events = POLLIN | POLLOUT;
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