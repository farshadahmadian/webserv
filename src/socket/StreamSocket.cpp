/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:31:58 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/16 15:06:17 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StreamSocket.hpp"

StreamSocket::SocketException::SocketException(void) : std::exception(), _errMsg("") {
	return;
}

StreamSocket::SocketException::~SocketException(void) throw(){
	return;
}

StreamSocket::SocketException::SocketException(std::string errMsg) : std::exception(), _errMsg(errMsg) , _error(NO_ERROR) {
	return;
}

char const *StreamSocket::SocketException::what() const throw() {
	if (_errMsg.empty())
		return "Socket creation failed\n";
	return this->_errMsg.c_str();
}


StreamSocket::StreamSocket(void) : _port("8080"), _ip("127.0.0.1") {
	
	this->_addrInfo = this->initAddrInfo(); // _addrInfo must be deleted in the destructor
	this->_socketFd = this->createSocket();
	this->setPortAvailable();
	this->bindSocket();
}

t_error StreamSocket::SocketException::getError(void) const{
	return this->_error;
}

void StreamSocket::SocketException::setError(t_error error) {
	this->_error = error;
	return;
}

struct addrinfo *StreamSocket::initAddrInfo(void) {

	struct addrinfo hints;
	struct addrinfo *addrInfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int getAddrResult = getaddrinfo((this->_ip).c_str(), (this->_port).c_str(), &hints, &addrInfo);
	if (getAddrResult != 0)
	{
		std::string errMsg = static_cast<std::string>(gai_strerror(getAddrResult));
		StreamSocket::SocketException socketException(errMsg);
		socketException.setError(GET_ADDR_INFO_FAILD);
		throw socketException;
	}
	return addrInfo;
}

int StreamSocket::createSocket(void) {
	int socketFd = socket(this->_addrInfo->ai_family, this->_addrInfo->ai_socktype, this->_addrInfo->ai_protocol);
	if (socketFd == -1)
	{
		StreamSocket::SocketException socketException("Socket creation faild!");
		socketException.setError(SOCKET_CREATION_FAILD);
		throw socketException;
	}
	return socketFd;
}

void StreamSocket::setPortAvailable(void) {
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	return;
}

void StreamSocket::bindSocket(void) {
	int bindResult = bind(this->_socketFd, this->_addrInfo->ai_addr, this->_addrInfo->ai_addrlen);
	if (bindResult != 0)
	{
		StreamSocket::SocketException socketException("Binding the socket to the address failed!");
		socketException.setError(BIND_SOCKET_FAILD);
		throw socketException;
	}
	return;
}
