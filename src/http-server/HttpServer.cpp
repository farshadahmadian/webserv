/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/01 09:55:43 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(void) : _listeningSocket(ListeningSocket()) {
	return;
}

HttpServer::HttpServer(HttpServer const &other) : _listeningSocket(other._listeningSocket) {
	return;
}

HttpServer::HttpServer(int maxIncomingConnections, std::string const &ip, std::string const &port) : _listeningSocket(maxIncomingConnections, ip, port) {
	return;
}

HttpServer &HttpServer::operator=(HttpServer const &rhs) {
	if (this != &rhs)
		this->_listeningSocket = rhs._listeningSocket;
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
	int listenResult = listen(this->_listeningSocket.getSocketFd(), this->_listeningSocket.getMaxIncomingConnections());
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
	
	std::cout << GREEN << "Listening socket is litening to requests" << RESET << std::endl;
	return;
}

void HttpServer::acceptFirstRequestInQueue(void) {
	
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

	return;
}
