/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:10 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/11 09:49:47 by fahmadia         ###   ########.fr       */
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
	_connectionStartTime(std::time(NULL)),
	_state(CREATED),
	_request(""),
	_contentLength(0),
	_requestBody("") {
	return;
}

ConnectedSocket::ConnectedSocket(ConnectedSocket const &other): 
	Socket(other), 
	_incomingRequestAddress(other._incomingRequestAddress), 
	_incomingConnectionAddressSize(other._incomingConnectionAddressSize),
	_isConnected(other._isConnected),
	_iterationNum(other._iterationNum),
	_connectionStartTime(other._connectionStartTime),
	_state(other._state),
	_request(other._request),
	_contentLength(other._contentLength),
	_requestBody(other._requestBody) {
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
		this->_state = rhs._state;
		this->_request = rhs._request;
		this->_contentLength = rhs._contentLength;
		this->_requestBody = rhs._requestBody;
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

int ConnectedSocket::getIterationNum() {
	return this->_iterationNum;
}

t_state ConnectedSocket::getState(void) {
	return this->_state;
}

std::string const &ConnectedSocket::getRequest(void)const {
	return this->_request;
}

std::string const &ConnectedSocket::appendToRequest(std::string toAppend) {
	return this->_request.append(toAppend);
}

std::string const &ConnectedSocket::appendToBody(std::string toAppend) {
	return this->_requestBody.append(toAppend);
}

size_t ConnectedSocket::getContentLength(void) {
	return this->_contentLength;
}

std::string const &ConnectedSocket::getRequestBody(void) const {
	return this->_requestBody;
}

void ConnectedSocket::setRequestBodyLength(std::string contentLength) {
	std::istringstream inputStream;
	inputStream.str(contentLength);
	size_t bodyLength;
	inputStream >> bodyLength;
	this->_contentLength = bodyLength;
	return;
}

void ConnectedSocket::setState(t_state state) {
	this->_state = state;
}

void ConnectedSocket::setConnectionStartTime() {
	this->_connectionStartTime = std::time(NULL);
	return;
}

void ConnectedSocket::clearRequestProperties(void) {
	this->_request = "";
	this->_contentLength = 0;
	this->_requestBody = "";
}

void ConnectedSocket::setIterationNum(int iterationNum) {
	this->_iterationNum = iterationNum;
	return;
}