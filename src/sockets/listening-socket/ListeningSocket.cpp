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


#include <fcntl.h>
#include "Server.hpp"
#include "ListeningSocket.hpp"

// change default values
// should not be used
ListeningSocket::ListeningSocket(void):
	Socket(),
	_maxIncomingConnections(10),
	_ip("127.0.0.1"),
	_port("8080"),
	_addressInfo(NULL) {

	try {
		this->_addressInfo = this->allocateAddrInfo();
		int socketFd = this->createSocket();
		this->setSocketFd(socketFd);
	} catch(Exception const &exception) { // if an Exception instance were thrown, catch it, and again throw and then in the main function it will be caught in the try-catch block
		throw exception;
	}
	return;
}

ListeningSocket::ListeningSocket( unsigned int maxIncomingConnections, std::string const& ip, std::string const& port ):
	Socket(),
	_maxIncomingConnections(maxIncomingConnections),
	_ip(ip),
	_port(port),
	_addressInfo(NULL) {

	try {
		this->_addressInfo = this->allocateAddrInfo();
		int socketFd = this->createSocket();
		if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1) {
	      perror("fcntl F_SETFL");
		}
		this->setSocketFd(socketFd);
	} catch (Exception const &exception) {
		Server::logMessage("ERROR: " + std::string(exception.what()));
		throw exception;
	}
	return;
}

ListeningSocket::ListeningSocket( ListeningSocket const &other ):
	Socket(other),
	_maxIncomingConnections(other._maxIncomingConnections),
	_ip(other._ip),
	_port(other._port),
	_addressInfo(NULL) {

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

struct addrinfo *ListeningSocket::allocateAddrInfo( void ) const {
	struct addrinfo hints;
	struct addrinfo *addrInfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int getAddrResult = getaddrinfo((this->_ip).c_str(), (this->_port).c_str(), &hints, &addrInfo);
	if (getAddrResult != 0)
	{
		std::string errMsg = static_cast<std::string>(gai_strerror(getAddrResult));
		// Server::logMessage("ERROR: " + errMsg);
		Exception exception(errMsg, GET_ADDR_INFO_FAILED);
		throw exception;
	}
	return (addrInfo);
}

int ListeningSocket::createSocket(void) const {
	struct addrinfo *temp;
	int socketFd = -1;

	// std::cout << "this->_addressInfo->ai_family = " << this->_addressInfo->ai_family << std::endl;
	// std::cout << "this->_addressInfo->ai_socktype = " << this->_addressInfo->ai_socktype << std::endl;
	// std::cout << "this->_addressInfo->ai_protocol =" << this->_addressInfo->ai_protocol << std::endl;

	for (temp = this->_addressInfo; temp != NULL; temp = temp->ai_next)
	{
		socketFd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socketFd == -1)
		{
			// Exception exception("Socket creation faild!", GET_ADDR_INFO_FAILED);
			// throw exception;
			continue;
		}
		else
			break;

	}

	if (socketFd == -1)
		{		
			Exception exception("Socket creation faild!", GET_ADDR_INFO_FAILED);
			throw exception;
		}
	// freeaddrinfo(this->_addressInfo);
	return socketFd;
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

unsigned int ListeningSocket::getMaxIncomingConnections(void) const {
	return this->_maxIncomingConnections;
}

void ListeningSocket::setAddressInfo(addrinfo *addressInfo) {
	this->_addressInfo = addressInfo;
}
