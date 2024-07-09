/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 16:26:11 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void): 
	_listeningSocket(ListeningSocket()), 
	_connectedSockets(std::map<int, ConnectedSocket>()), 
	_maxIncomingConnections(10), 
	_monitoredFdsNum(0), 
	_monitoredFds(NULL), 
	_request(std::map<std::string, std::string>()) {

	this->_monitoredFds = new struct pollfd[this->_maxIncomingConnections + 1];
	memset(this->_monitoredFds, 0, sizeof(struct pollfd) * (this->_maxIncomingConnections + 1));
	for (unsigned int i = 0; i < this->_maxIncomingConnections + 1; i++)
	{
		this->_monitoredFds[i].fd = -1;
	}
	this->_monitoredFds[0].fd = this->_listeningSocket.getSocketFd();
	this->_monitoredFds[0].events = POLLIN;
	this->_monitoredFdsNum++;
	return;
}

Server::Server( std::map<std::string, std::string> settings ) { // new from nico
	(void) settings;
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
	// from the map of settings to set the real options for the server
}

Server::Server(Server const &other) : _listeningSocket(other._listeningSocket),  _connectedSockets(other._connectedSockets), _maxIncomingConnections(other._maxIncomingConnections), _monitoredFdsNum(other._monitoredFdsNum), _monitoredFds(NULL) {

	// deep copy _monitoredFds
	// _request
	return;
}

// Server::Server(Server const &other) {
// 	*this = other;
// 	return;
// }

Server::Server(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port) : _listeningSocket(maxIncomingConnections, ip, port),  _connectedSockets(std::map<int, ConnectedSocket>()), _maxIncomingConnections(maxIncomingConnections), _monitoredFdsNum(0), _monitoredFds(NULL), _request(std::map<std::string, std::string>()) {

	this->_monitoredFds = new struct pollfd[this->_maxIncomingConnections + 1];
	memset(this->_monitoredFds, 0, sizeof(struct pollfd) * (this->_maxIncomingConnections + 1));
	for (unsigned int i = 0; i < this->_maxIncomingConnections + 1; i++)
	{
		this->_monitoredFds[i].fd = -1;
	}
	this->_monitoredFds[0].fd = this->_listeningSocket.getSocketFd();
	this->_monitoredFds[0].events = POLLIN;
	this->_monitoredFdsNum++;
	return;
}

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs)
	{
		this->_listeningSocket = rhs._listeningSocket;
		this->_connectedSockets = rhs._connectedSockets;
		this->_monitoredFdsNum = rhs._monitoredFdsNum;
		this->_maxIncomingConnections = rhs._maxIncomingConnections;
		//deep copy _monitorFds
		//_request
	}
	return *this;
}

Server::~Server(void) {
	// close(this->_listeningSocket.getSocketFd());

	// std::map<int, ConnectedSocket>::iterator iterator;
	// std::map<int, ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	// for (iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
	// 	close(iterator->second.getSocketFd());

	delete this->_monitoredFds;

	return;
}

ListeningSocket const &Server::getListeningSocket(void) const {
	return this->_listeningSocket;
}

void Server::printConnectedSockets(void) {
	std::map<int, ConnectedSocket>::iterator iterator;
	std::map<int, ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	std::cout << "Connected Sockets List:" << std::endl;
	for(iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
		std::cout << "connectedSocket.key = " << iterator->first << " connectedSocket.value = " << iterator->second.getSocketFd() << std::endl;
}

void Server::setPortAvailable(void) {
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->_listeningSocket.getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	if (setSocketOptionResult == -1)
	{
		Exception exception("Setting socket options faild!", SOCKET_OPTIONS_FAILD);
		throw exception;
	}
	std::cout << GREEN << "Listening Socket with fd("<< this->_listeningSocket.getSocketFd() <<") of type " << (this->_listeningSocket.getAddressInfo()->ai_socktype == SOCK_STREAM ? "TCP Socket" : "UNKNOWN SOCKET!!!!") << " created" << RESET << std::endl;
	return;
}

void Server::bindSocket(void) const {
	int bindResult = bind(this->_listeningSocket.getSocketFd(), this->_listeningSocket.getAddressInfo()->ai_addr, this->_listeningSocket.getAddressInfo()->ai_addrlen);
	if (bindResult != 0)
	{
		Exception exception("Binding the socket to the address failed!", BIND_SOCKET_FAILD);
		throw exception;
	}
	
	// char *ip = new char[100];
	// char ip[100];
	// inet_ntop(this->_addressInfo->ai_family, &((reinterpret_cast<sockaddr_in *>(this->_addressInfo->ai_addr))->sin_addr), ip, 100);
	std::string ip = static_cast<std::string>(inet_ntoa(reinterpret_cast<sockaddr_in *>(this->_listeningSocket.getAddressInfo()->ai_addr)->sin_addr)); // REMOVE
	std::cout << GREEN << "Listening Socket is bound to " << ip.c_str() <<":" << ntohs(reinterpret_cast<sockaddr_in *>(this->_listeningSocket.getAddressInfo()->ai_addr)->sin_port) << RESET << std::endl;

	return;
}

void Server::listenToRequests(void) const {
	int listenResult = listen(this->_listeningSocket.getSocketFd(), static_cast<int>(this->_listeningSocket.getMaxIncomingConnections()));
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
	
	std::cout << GREEN << "Listening socket is litening to requests" << RESET << std::endl;
	return;
}

int Server::acceptFirstRequestInQueue(void) {
	
	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	int connectedSocketFd = accept(this->_listeningSocket.getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);

	if (connectedSocket.getSocketFd() == -1) {
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}

	this->_connectedSockets[connectedSocketFd] = connectedSocket;

	std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") is created" << RESET << std::endl; 

	return connectedSocketFd;
}

void Server::startPoll(void) {
	if (this->_monitoredFds[0].fd == -1) {
		this->_monitoredFds[0].fd = _listeningSocket.getSocketFd();
		this->_monitoredFds[0].events = POLLIN; this->_monitoredFdsNum++;
	}

	int eventsNum = poll(this->_monitoredFds, this->_monitoredFdsNum, 5000);

	if (eventsNum < 0)
	{
		Exception pollException("Error ocurred in poll", POLL_FAILED);
		throw pollException;
	}
	
	if (eventsNum == 0)
		std::cout << "Timeout! No event received" << std::endl;
	else if (eventsNum > 0) {
		
		for (unsigned int i = 0; i < this->_monitoredFdsNum; i++)
		{
			short int reventResult = this->_monitoredFds[i].revents & POLLIN;
			int connectedSocketFd = -1;

			if (reventResult != 0) {
			std::cout << "Listening socket wiht fd " << this->_monitoredFds[i].fd << " has data to be read" << std::endl;
			
			if (this->_monitoredFdsNum <= this->_maxIncomingConnections && i == 0) {
				connectedSocketFd = this->acceptFirstRequestInQueue();
				this->_monitoredFdsNum++;
				for (unsigned int j = 0; j < this->_monitoredFdsNum; j++)
				{
				std::cout << "_monitoredFdsNum = " << _monitoredFdsNum << std::endl;
					if (this->_monitoredFds[j].fd == -1) {
					this->_monitoredFds[j].fd = connectedSocketFd;
					this->_monitoredFds[j].events = POLLIN;
					}
				}
				
				
				} else if (i != 0) {
					// char string[100];
					std::cout << "this->_monitoredFds[" << i << "].revents = " << this->_monitoredFds[i].revents << std::endl;
					// recv(connectedSocketFd, string, 100, MSG_FLUSH);
					close(this->_monitoredFds[i].fd);
					this->_monitoredFds[i].fd = -1;
					this->_monitoredFds[i].revents = 0;
					this->_monitoredFdsNum--;
					this->_connectedSockets.erase(_monitoredFds[i].fd);
				} 
				// else {
				// 	std::cout << "Maximum connections number reached. Can't accept any more connections!" << std::endl;
				// 	this->_monitoredFds[0].revents = 0;
				// 	this->_monitoredFds[0].fd = -1;
				// 	this->_monitoredFdsNum--;
				// 	break;
				// }
			}
		}
		// this->_monitoredFds[0].revents = 0;
		// this->_monitoredFds[0].fd = -1;
		// this->_monitoredFdsNum--;
	}
			
	return;
}

void Server::startPoll2(void) {
	// if (this->_monitoredFds[0].fd  == - 1)
	// {
		
	// }

	try {
		int eventsNum = poll(this->_monitoredFds, this->_monitoredFdsNum, 3000);

		if (eventsNum < 0) {
			Exception pollException("Poll exception", POLL_FAILED);
			throw pollException;
		}

		if (eventsNum == 0)
			std::cout << "Time's up, but no event occured on any monitored file descriptors!" << std::endl;

		if (eventsNum > 0)
		{
			handleEvents();
		}
	} catch(Exception const &exception) {
		throw exception;
	}

	// for (unsigned int i = 0; i < _monitoredFdsNum; i++)	{
		// handleListeningSocket();
		// handleConnectingSocket();
	// }
	return;
}


void Server::handleEvents(void) {
	try {
		for (unsigned int i = 0; i < this->_monitoredFdsNum; i++)
		{
			if (this->_monitoredFds[i].fd == this->_listeningSocket.getSocketFd())
			{
				std::cout << "handling event on the listening socket" << std::endl;
				this->handleEventsOnListeningSocket(i);
			}
			else {
				std::cout << "handling event on a connected socket" << std::endl;
				this->handleEventsOnConnectedSockets(i);
			}
		}
	}catch(Exception const &exception) {
		throw exception;
	}
	
}

void Server::handleEventsOnListeningSocket(unsigned int i) {
	try {
		// if ((this->_monitoredFds[i].revents & POLLERR) || (this->_monitoredFds[i].revents & POLLHUP) || (this->_monitoredFds[i].revents & POLLNVAL)) {
		// 	throw Exception("Event error", EVENT_ERROR);
		// }

		if ((this->_monitoredFds[i].revents & POLLIN) && (this->_monitoredFdsNum <= this->_maxIncomingConnections))	{
			int connectedSocketFd = this->acceptFirstRequestInQueue();
			this->addToMonitorsFds(connectedSocketFd);
			this->_monitoredFdsNum++;
		}
		// else {
		// 	throw Exception("Exception in listening socket!", EVENT_ERROR);
		// }
	} catch(Exception const &exception) {
		throw exception;
	}

	return;
}

void Server::addToMonitorsFds(int connectedSocketFd) {
	for (unsigned int i = 0; i <= this->_maxIncomingConnections ;i++) {
		if (this->_monitoredFds[i].fd == -1)
		{
			this->_monitoredFds[i].fd = connectedSocketFd;
			this->_monitoredFds[i].events = POLLIN | POLLOUT;
			return;
		}
	}
}

void Server::handleEventsOnConnectedSockets(unsigned int i) {
			std::cout << "this->_monitoredFds[" << i << "].fd = "  << this->_monitoredFds[i].fd << std::endl;
			std::cout << "this->_monitoredFds[i].revents = "  << std::hex << "0x" << (this->_monitoredFds[i].revents) << std::dec << std::endl;
			std::cout << "this->_monitoredFds[i].revents & POLLIN = "  << std::hex << "0x" << (this->_monitoredFds[i].revents & POLLIN) << std::dec << std::endl;
			std::cout << "this->_monitoredFds[i].revents & POLOUT = "  << std::hex << "0x" << (this->_monitoredFds[i].revents & POLLOUT) << std::dec << std::endl;
	try {
		if (this->_monitoredFds[i].revents & POLLIN) {
			char receive[20048];
			receive[20047] = '\0';
			ssize_t result = recv(this->_monitoredFds[i].fd, receive, sizeof(receive) - 1, 0);
			std::cout << "received request:\n" << GREEN << receive << RESET << std::endl;
			if (result == -1)
				throw Exception("Receive Failed", RECEIVE_FAILED);
			// close(this->_monitoredFds[i].fd);
			// this->_monitoredFdsNum--;
			this->parseRequest(static_cast<std::string>(receive));
			this->printRequest();

		}

		if (this->_monitoredFds[i].revents & POLLOUT) {
			std::cout << "sending the response\n";
			std::string htmlContent = this->readHtmlFile("./src/index.html");
			std::ostringstream ss;
			ss << "HTTP/1.1 200 OK\r\n";
			ss << "Content-Type: text/html\r\n";
			ss << "Content-Length:" << htmlContent.size() << "\r\n";
			ss << "\r\n";
			ss << htmlContent;
			size_t size = ss.str().size();
			// std::cout << ss.str() << std::endl;
			send(this->_monitoredFds[i].fd, ss.str().c_str(), size, 0);
			close(this->_monitoredFds[i].fd);
			this->_monitoredFdsNum--;
		}
		// else {
		// 	throw Exception("Exception in connected socket!", EVENT_ERROR);
		// }
	}catch(Exception const &exception) {
		throw exception;
	}
	return;
}

std::string Server::readHtmlFile(std::string path) {
	std::ifstream fileStream;
	fileStream.open(path.c_str());
	if (fileStream.is_open())
		std::cout << "file is open\n";
	else
		perror("error:");
	std::ostringstream ss;
	ss << fileStream.rdbuf();
	// std::cout << ss.str() << std::endl;
	return ss.str();
}

void Server::parseRequest(std::string request) {
	std::istringstream inputStringStream(request, std::ios::in);
	std::cout << "Parsing the request:" << std::endl;
	// std::cout << inputStringStream.str() << std::endl;
	std::string method, path, httpVersion;
	inputStringStream >> method;
	inputStringStream >> path;
	inputStringStream >> httpVersion;
	// std::cout << "Method = " << method << std::endl;
	// std::cout << "path = " << path << std::endl;
	// std::cout << "httpVersion = " << httpVersion << std::endl;

	this->_request["method"] = method;
	this->_request["path"] = path;
	this->_request["httpVersion"] = httpVersion;
}

void Server::printRequest(void) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_request.end();

	for (iterator = this->_request.begin(); iterator != iteratorEnd; iterator++)
		std::cout << iterator->first << " = " << iterator->second << std::endl;
	return;
}
