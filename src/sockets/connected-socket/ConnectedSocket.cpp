/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:10 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/03 15:09:15 by fahmadia         ###   ########.fr       */
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
	_requestBody(std::ios::binary | std::ios::app),
	_requestHeader(""),
	_isHeaderComplete(false),
	_requestMap(std::map<std::string, std::string>()),
	_avoidBodyFirstChunckRepeat(false),
	_cgiStartTime(std::time(NULL)),
	_isCgi(false),
	_isCgiChildProcessReturning(false),
	_isCgiChildProcessSuccessful(false),
	_cgiScriptExtension("") {
		this->_childProcessData.id = -1;
		this->_childProcessData.pipeFds[0] = -1;
		this->_childProcessData.pipeFds[1] = -1;
		this->_childProcessData.isError = false;

		// this->_isCgiChildProcessReturning = false;
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
	_requestHeader(other._requestHeader),
	_isHeaderComplete(other._isHeaderComplete),
	_requestMap(other._requestMap),
	_avoidBodyFirstChunckRepeat(other._avoidBodyFirstChunckRepeat),
	_cgiStartTime(other._cgiStartTime),
	_isCgi(other._isCgi),
	_childProcessData(other._childProcessData) {
	this->_requestBody << other._requestBody.rdbuf();

	this->_isCgiChildProcessReturning = other._isCgiChildProcessReturning;
	this->_isCgiChildProcessSuccessful = other. _isCgiChildProcessSuccessful;
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
		this->_requestBody << rhs._requestBody.rdbuf();
		this->_requestHeader = rhs._requestHeader;
		this->_isHeaderComplete = rhs._isHeaderComplete;
		this->_requestMap = rhs._requestMap;
		this->_avoidBodyFirstChunckRepeat = rhs._avoidBodyFirstChunckRepeat;
		this->_cgiStartTime = rhs._cgiStartTime;
		this->_isCgi = rhs._isCgi;
		this->_childProcessData = rhs._childProcessData;

		this->_isCgiChildProcessReturning = rhs._isCgiChildProcessReturning;
		this->_isCgiChildProcessSuccessful = rhs._isCgiChildProcessSuccessful;
	}
	return *this;
}

ConnectedSocket::~ConnectedSocket(void) {
	// if (this->_requestBody.is_open()) {
	// 	this->_requestBody.clear();
	// 	this->_requestBody.close();
	// }
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

std::ostringstream const &ConnectedSocket::getRequestBody(void) const {
	return this->_requestBody;
}

std::string const &ConnectedSocket::getRequestHeader(void) const {
	return this->_requestHeader;
}

size_t ConnectedSocket::getContentLength(void) {
	return this->_contentLength;
}

bool ConnectedSocket::getIsHeaderComplete(void) {
	return this->_isHeaderComplete;
}

std::map<std::string, std::string> &ConnectedSocket::getRequestMap(void) {
	return this->_requestMap;
}

bool ConnectedSocket::getAvoidBodyFirstChunckRepeat(void) {
	return this->_avoidBodyFirstChunckRepeat;
}

void ConnectedSocket::setRequestBodyLength(std::string contentLength) {
	if (contentLength.empty())
	{
		this->_contentLength = 0;
		return;
	}
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

void ConnectedSocket::setIsHeaderComplete(bool isHeaderComplete) {
	this->_isHeaderComplete = isHeaderComplete;
	return;
}

void ConnectedSocket::setRequestMap(std::map<std::string, std::string> const &requestMap) {
	this->_requestMap = requestMap;
}

void ConnectedSocket::setAvoidBodyFirstChunckRepeat(bool isBodyFirstChunckReceived) {
	this->_avoidBodyFirstChunckRepeat = isBodyFirstChunckReceived;
}

std::string const &ConnectedSocket::appendToRequest(std::string const &toAppend) {
	return this->_request.append(toAppend);
}

std::ostringstream const &ConnectedSocket::appendToBody(std::ostringstream const &outputStringStream) {
	this->_requestBody.clear();
	this->_requestBody.write(outputStringStream.str().c_str(), outputStringStream.str().size());
	this->_requestBody.flush();
	return this->_requestBody;
}

std::string const &ConnectedSocket::appendToHeader(std::string const &toAppend) {
	return this->_requestHeader.append(toAppend);
}

void ConnectedSocket::clearRequestProperties(void) {
	this->_request = "";
	this->_contentLength = 0;
	this->_requestBody.flush();
	this->_requestBody.clear();
	this->_requestBody.str("");
	this->_requestHeader = "";
	this->_isHeaderComplete = false;
	this->_requestMap.clear();
}

void ConnectedSocket::setIterationNum(int iterationNum) {
	this->_iterationNum = iterationNum;
	return;
}

time_t const &ConnectedSocket::getCgiStartTime(void) const {
	return this->_cgiStartTime;
}

void ConnectedSocket::setCgiStartTime() {
	this->_cgiStartTime = std::time(NULL);
}

bool ConnectedSocket::getIsCgi(void) const {
	return this->_isCgi;
}

void ConnectedSocket::setIsCgi(bool isCgi) {
	this->_isCgi = isCgi;
}