/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/06 13:03:01 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include "HttpServer.hpp"

HttpServer::HttpServer(void) : _listeningSocket(ListeningSocket()), _connectedSockets(std::vector<ConnectedSocket>()), _maxIncomingConnections(3), _monitoredFdsNum(1), _monitoredFds(NULL) {

	this->_monitoredFds = new struct pollfd[this->_maxIncomingConnections + 1];
	for (unsigned int i = 0; i < this->_maxIncomingConnections + 1; i++)
	{
		this->_monitoredFds[i].fd = -1;
	}
	this->_monitoredFds[0].fd = this->_listeningSocket.getSocketFd();
	this->_monitoredFds[0].events = POLLIN;
	return;
}

HttpServer::HttpServer(HttpServer const &other) : _listeningSocket(other._listeningSocket),  _connectedSockets(other._connectedSockets), _maxIncomingConnections(other._maxIncomingConnections), _monitoredFdsNum(other._monitoredFdsNum), _monitoredFds(NULL) {

	// deep copy _monitoredFds
	return;
}

// HttpServer::HttpServer(HttpServer const &other) {
// 	*this = other;
// 	return;
// }

HttpServer::HttpServer(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port) : _listeningSocket(maxIncomingConnections, ip, port),  _connectedSockets(std::vector<ConnectedSocket>()), _maxIncomingConnections(maxIncomingConnections), _monitoredFdsNum(1), _monitoredFds(NULL) {

	this->_monitoredFds = new struct pollfd[this->_maxIncomingConnections + 1];
	for (unsigned int i = 0; i < this->_maxIncomingConnections + 1; i++)
	{
		this->_monitoredFds[i].fd = -1;
	}
	this->_monitoredFds[0].fd = this->_listeningSocket.getSocketFd();
	this->_monitoredFds[0].events = POLLIN;
	return;
}

HttpServer &HttpServer::operator=(HttpServer const &rhs) {
	if (this != &rhs)
	{
		this->_listeningSocket = rhs._listeningSocket;
		this->_connectedSockets = rhs._connectedSockets;
		this->_monitoredFdsNum = rhs._monitoredFdsNum;
		this->_maxIncomingConnections = rhs._maxIncomingConnections;
		//deep copy _monitorFds
	}
	return *this;
}

HttpServer::~HttpServer(void) {
	close(this->_listeningSocket.getSocketFd());

	std::vector<ConnectedSocket>::iterator iterator;
	std::vector<ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	for (iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
		close(iterator->getSocketFd());

	return;
}

ListeningSocket const &HttpServer::getListeningSocket(void) const {
	return this->_listeningSocket;
}

void HttpServer::printConnectedSockets(void) {
	std::vector<ConnectedSocket>::iterator iterator;
	std::vector<ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	std::cout << "Connected Sockets List:" << std::endl;
	for(iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
		std::cout << "Socket fd = " << iterator->getSocketFd() << std::endl;
}

void HttpServer::setPortAvailable(void) {
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

void HttpServer::bindSocket(void) const {
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

void HttpServer::listenToRequests(void) const {
	int listenResult = listen(this->_listeningSocket.getSocketFd(), static_cast<int>(this->_listeningSocket.getMaxIncomingConnections()));
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
	
	std::cout << GREEN << "Listening socket is litening to requests" << RESET << std::endl;
	return;
}

int HttpServer::acceptFirstRequestInQueue(void) {
	
	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	int connectedSocketFd = accept(this->_listeningSocket.getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);

	if (connectedSocket.getSocketFd() == -1) {
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}

	this->_connectedSockets.push_back(connectedSocket);

	std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") is created" << RESET << std::endl; 

	return connectedSocketFd;
}

void HttpServer::startPoll(void) {
	// if this->_monitoredFds[0].fd == -1 ==> this->_monitoredFds[0].fd = listeningSocket.fd; this->_monitoredFds[0].fd.event = POLLIN;

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
				
				} else if (i != 0){
					// char string[100];
					std::cout << "this->_monitoredFds[" << i << "].revents = " << this->_monitoredFds[i].revents << std::endl;
					// recv(connectedSocketFd, string, 100, MSG_FLUSH);
					close(this->_monitoredFds[i].fd);
					this->_monitoredFds[i].fd = -1;
					this->_monitoredFds[i].revents = 0;
					this->_monitoredFdsNum--;
					
				} else {
					std::cout << "Maximum connections number reached. Can't accept any more connections!" << std::endl;
					this->_monitoredFds[0].revents = 0;
					this->_monitoredFds[0].fd = -1;

					break;
				}
			}
		}

	}
			
	return;
}
