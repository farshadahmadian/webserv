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

#include "ListeningSocket.hpp"


// change _maxIncomingConnections(10)
ListeningSocket::ListeningSocket(void) : Socket(), _port("8080"), _ip("127.0.0.1"), _maxIncomingConnections(10) {
	try {
		this->_addrInfo = this->initAddrInfo();
		this->_socketFd = this->createSocket();
	} catch(ListeningSocket::SocketException const &socketException) {
		throw socketException;
	}
}

ListeningSocket::~ListeningSocket(void) {
	close(this->_socketFd);
	freeaddrinfo(this->_addrInfo);
}

t_error ListeningSocket::SocketException::getError(void) const{
	return this->_error;
}

void ListeningSocket::SocketException::setError(t_error error) {
	this->_error = error;
	return;
}

struct addrinfo *ListeningSocket::initAddrInfo(void) {
	struct addrinfo hints;
	struct addrinfo *addrInfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int getAddrResult = getaddrinfo((this->_ip).c_str(), (this->_port).c_str(), &hints, &addrInfo);
	if (getAddrResult != 0)
	{
		std::string errMsg = static_cast<std::string>(gai_strerror(getAddrResult));
		ListeningSocket::SocketException socketException(errMsg);
		socketException.setError(GET_ADDR_INFO_FAILD);
		throw socketException;
	}
	return addrInfo;
}

int ListeningSocket::createSocket(void) {
	int socketFd = socket(this->_addrInfo->ai_family, this->_addrInfo->ai_socktype, this->_addrInfo->ai_protocol);
	if (socketFd == -1)
	{
		ListeningSocket::SocketException socketException("Socket creation faild!");
		socketException.setError(SOCKET_CREATION_FAILD);
		throw socketException;
	}
	return socketFd;
}

void ListeningSocket::setPortAvailable(void) {
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	if (setSocketOptionResult == -1)
	{
		ListeningSocket::SocketException socketException("Setting socket options faild!");
		socketException.setError(SOCKET_OPTIONS_FAILD);
		throw socketException;
	}
	return;
}

void ListeningSocket::bindSocket(void) {
	int bindResult = bind(this->_socketFd, this->_addrInfo->ai_addr, this->_addrInfo->ai_addrlen);
	if (bindResult != 0)
	{
		ListeningSocket::SocketException socketException("Binding the socket to the address failed!");
		socketException.setError(BIND_SOCKET_FAILD);
		throw socketException;
	}
	return;
}

void ListeningSocket::listenToRequests(void) {
	int listenResult = listen(this->_socketFd, this->_maxIncomingConnections);
	if (listenResult == -1)
	{
		ListeningSocket::SocketException socketException("Listening to incoming connections failed!");
		socketException.setError(LISTENING_FAILED);
		throw socketException;
	}
}

ListeningSocket ListeningSocket::acceptFirstRequestInQueue(void) {
	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	ListeningSocket newSocket;

	newSocket._socketFd = accept(this->_socketFd, reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	if (newSocket._socketFd == -1) {
		ListeningSocket::SocketException socketException("Accepting the request failed");
		socketException.setError(ACCEPTING_FAILED);
		throw socketException;
	}
	return newSocket;
}