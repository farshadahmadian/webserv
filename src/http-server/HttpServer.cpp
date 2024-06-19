/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/19 15:05:54 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(void) : _port("8080"), _ip("127.0.0.1"), _listeningSocket(ListeningSocket(10)) {
	try {
		this->_addressInfo = this->initAddrInfo();
		int socketFd = this->createSocket();
		this->_listeningSocket.setSocketFd(socketFd);
		this->_listeningSocket.setAddressInfo(this->_addressInfo);
	} catch(Exception const &exception) {
		throw exception;
	}
}

HttpServer::~HttpServer(void) {
	freeaddrinfo(this->_addressInfo);
	return;
}

struct addrinfo *HttpServer::initAddrInfo(void) {
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

int HttpServer::createSocket(void) {
	int socketFd = socket(this->_addressInfo->ai_family, this->_addressInfo->ai_socktype, this->_addressInfo->ai_protocol);
	if (socketFd == -1)
	{
		Exception exception("Socket creation faild!", GET_ADDR_INFO_FAILD);
		throw exception;
	}
	return socketFd;
}

ListeningSocket const &HttpServer::getListeningSocket(void) const {
	return this->_listeningSocket;
}
