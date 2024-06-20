/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:31:58 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/18 10:12:25 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "ListeningSocket.hpp"


// change default values
ListeningSocket::ListeningSocket(void) : Socket(), _maxIncomingConnections(10), _ip("127.0.0.1"), _port("8080"), _addressInfo(NULL) {

	try {	
		this->_addressInfo = this->allocateAddrInfo();
		int socketFd = this->createSocket();
		this->setSocketFd(socketFd);
	} catch(Exception const &exception) { // if an Exception instance were thrown, catch it, and again throw and then in the main function it will be caught in the try-catch block
		throw exception;
	}
	return;
}

ListeningSocket::ListeningSocket(int maxIncomingConnections, std::string const &ip, std::string const &port) : Socket(), _maxIncomingConnections(maxIncomingConnections), _ip(ip), _port(port), _addressInfo(NULL) {
	this->_addressInfo = this->allocateAddrInfo();
	int socketFd = this->createSocket();
	this->setSocketFd(socketFd);
	return;
}

ListeningSocket::ListeningSocket(ListeningSocket const &other) : Socket(other),   _maxIncomingConnections(other._maxIncomingConnections), _ip(other._ip), _port(other._port), _addressInfo(NULL) {

	// getaddrinfo((other._ip).c_str(), other._port.c_str(), other._addressInfo, &(this->_addressInfo));
	this->_addressInfo = other.allocateAddrInfo();
	return;
}

ListeningSocket &ListeningSocket::operator=(ListeningSocket const &rhs) {
	if (this != &rhs) {
		this->setSocketFd(rhs.getSocketFd());
		this->_maxIncomingConnections = rhs._maxIncomingConnections;
		this->_ip = rhs._ip;
		this->_port = rhs._port;
		if (this->_addressInfo)
			freeaddrinfo(this->_addressInfo);
		this->_addressInfo = rhs.allocateAddrInfo();
	}
	return *this;
}

ListeningSocket::~ListeningSocket(void) {
	freeaddrinfo(this->_addressInfo);
	return;
}

struct addrinfo *ListeningSocket::allocateAddrInfo(void) const {
	struct addrinfo hints;
	struct addrinfo *addrInfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int getAddrResult = getaddrinfo((this->_ip).c_str(), (this->_port).c_str(), &hints, &addrInfo);
	if (getAddrResult != 0)
	{
		std::string errMsg = static_cast<std::string>(gai_strerror(getAddrResult));
		Exception exception(errMsg, GET_ADDR_INFO_FAILD);
		throw exception;
	}
	return addrInfo;
}

int ListeningSocket::createSocket(void) const {
	int socketFd = socket(this->_addressInfo->ai_family, this->_addressInfo->ai_socktype, this->_addressInfo->ai_protocol);
	if (socketFd == -1)
	{
		Exception exception("Socket creation faild!", GET_ADDR_INFO_FAILD);
		throw exception;
	}
	return socketFd;
}

void ListeningSocket::setPortAvailable(void) const {
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	if (setSocketOptionResult == -1)
	{
		Exception exception("Setting socket options faild!", SOCKET_OPTIONS_FAILD);
		throw exception;
	}
	std::cout << GREEN << "Listening Socket with fd("<< this->getSocketFd() <<") of type " << (this->_addressInfo->ai_socktype == SOCK_STREAM ? "TCP Socket" : "UNKNOWN SOCKET!!!!") << " created" << RESET << std::endl;
	return;
}

void ListeningSocket::bindSocket(void) const {
	int bindResult = bind(this->getSocketFd(), this->_addressInfo->ai_addr, this->_addressInfo->ai_addrlen);
	if (bindResult != 0)
	{
		Exception exception("Binding the socket to the address failed!", BIND_SOCKET_FAILD);
		throw exception;
	}
	
	// char *ip = new char[100];
	// char ip[100];
	// inet_ntop(this->_addressInfo->ai_family, &((reinterpret_cast<sockaddr_in *>(this->_addressInfo->ai_addr))->sin_addr), ip, 100);
	std::string ip = static_cast<std::string>(inet_ntoa(reinterpret_cast<sockaddr_in *>(this->_addressInfo->ai_addr)->sin_addr)); // REMOVE
	std::cout << GREEN << "Listening Socket is bound to " << ip.c_str() <<":" << ntohs(reinterpret_cast<sockaddr_in *>(this->_addressInfo->ai_addr)->sin_port) << RESET << std::endl;

	return;
}

void ListeningSocket::listenToRequests(void) const {
	int listenResult = listen(this->getSocketFd(), this->_maxIncomingConnections);
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
	
	std::cout << GREEN << "Listening socket is litening to requests" << RESET << std::endl;
	return;
}

ConnectedSocket ListeningSocket::acceptFirstRequestInQueue(void) const {
	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	int connectedSocketFd = accept(this->getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);

	if (connectedSocket.getSocketFd() == -1) {
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}
	// connectedSocket.setSocketFd(connectedSocketFd);
	// std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") of type" << (connectedSocket._addressInfo->ai_socktype == SOCK_STREAM ? "TCP Scoket" : "UNKNOWN SOCKET!!!") << " is assigned to response to the incoming connection request" << RESET << std::endl; 

	std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") is created" << RESET << std::endl; 
	return connectedSocket;
}

addrinfo *ListeningSocket::getAddressInfo(void) const {
	return this->_addressInfo;
}

std::string const &ListeningSocket::getIp(void) const {
	return this->_ip;
}

std::string const &ListeningSocket::getPort(void) const {
	return this->_port;
}

void ListeningSocket::setAddressInfo(addrinfo *addressInfo) {
	this->_addressInfo = addressInfo;
}
