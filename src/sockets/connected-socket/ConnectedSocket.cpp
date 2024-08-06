/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:10 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/05 20:05:51 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectedSocket.hpp"

ConnectedSocket::ConnectedSocket(void) : Socket() {
	return;
}

ConnectedSocket::ConnectedSocket(int socketFd, sockaddr_storage const &incomingRequestAddress, socklen_t const &incomingConnectionAddressSize): 
	Socket(socketFd), 
	_incomingRequestAddress(incomingRequestAddress), 
	_incomingConnectionAddressSize(incomingConnectionAddressSize),
	_isConnected(true),
	_iterationNum(0),
	_connectionStartTime(std::time(NULL)) {
	return;
}

ConnectedSocket::ConnectedSocket(ConnectedSocket const &other): 
	Socket(other), 
	_incomingRequestAddress(other._incomingRequestAddress), 
	_incomingConnectionAddressSize(other._incomingConnectionAddressSize),
	_isConnected(other._isConnected),
	_iterationNum(0),
	_connectionStartTime(0) {
	return;
}

ConnectedSocket &ConnectedSocket::operator=(ConnectedSocket const &rhs) {
	if (this != &rhs)
	{
		this->setSocketFd(rhs.getSocketFd());
		this->_incomingRequestAddress = rhs._incomingRequestAddress;
		this->_incomingConnectionAddressSize = rhs._incomingConnectionAddressSize;
		this->_isConnected = rhs._isConnected;
		this->_iterationNum = rhs._iterationNum;
		this->_connectionStartTime = rhs._connectionStartTime;
	}
	return *this;
}

ConnectedSocket::~ConnectedSocket(void) {
	return;
}

sockaddr_storage const &ConnectedSocket::getIncomingRequestAddress(void) const {
	return this->_incomingRequestAddress;
}

socklen_t const &ConnectedSocket::getIncomingConnectionAddressSize(void) const {
	return this->_incomingConnectionAddressSize;
}

bool ConnectedSocket::getIsConnected(void) const {
	return this->_isConnected;
}

void ConnectedSocket::setIsConnected(bool isConnected) {
	this->_isConnected = isConnected;
}

time_t const &ConnectedSocket::getConnectionStartTime(void) const {
	return this->_connectionStartTime;
}

void ConnectedSocket::setConnectionStartTime() {
	this->_connectionStartTime = std::time(NULL);
	return;
}