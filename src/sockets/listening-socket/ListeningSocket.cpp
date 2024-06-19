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
ListeningSocket::ListeningSocket(void) : Socket(), _maxIncomingConnections(10), _addressInfo(NULL) {
	return;
}

ListeningSocket::ListeningSocket(int maxIncomingConnections) : Socket(), _maxIncomingConnections(maxIncomingConnections), _addressInfo(NULL) {
	return;
}

ListeningSocket::ListeningSocket(ListeningSocket const &other) : _maxIncomingConnections(other._maxIncomingConnections) {
	return;
}

ListeningSocket::~ListeningSocket(void) {
	return;
}

void ListeningSocket::setPortAvailable(void) const {
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	if (setSocketOptionResult == -1)
	{
		Exception exception("Setting socket options faild!", SOCKET_OPTIONS_FAILD);
		throw exception;
	}
	return;
}

void ListeningSocket::bindSocket(void) const {
	int bindResult = bind(this->getSocketFd(), this->_addressInfo->ai_addr, this->_addressInfo->ai_addrlen);
	if (bindResult != 0)
	{
		Exception exception("Binding the socket to the address failed!", BIND_SOCKET_FAILD);
		throw exception;
	}
	return;
}

void ListeningSocket::listenToRequests(void) const {
	int listenResult = listen(this->getSocketFd(), this->_maxIncomingConnections);
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
}

ListeningSocket ListeningSocket::acceptFirstRequestInQueue(void) const {
	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	ListeningSocket newSocket;

	int newSocketFd = accept(this->getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	if (newSocket.getSocketFd() == -1) {
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}
	newSocket.setSocketFd(newSocketFd);
	return newSocket;
}

addrinfo *ListeningSocket::getAddressInfo(void) const {
	return this->_addressInfo;
}

void ListeningSocket::setAddressInfo(addrinfo *addressInfo) {
	this->_addressInfo = addressInfo;
}
