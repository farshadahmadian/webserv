/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/20 19:49:59 by fahmadia         ###   ########.fr       */
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
	return;
}

ListeningSocket const &HttpServer::getListeningSocket(void) const {
	return this->_listeningSocket;
}

void HttpServer::runServer(void) {
	this->_listeningSocket.setPortAvailable();
	this->_listeningSocket.bindSocket();
	this->_listeningSocket.listenToRequests();
	ConnectedSocket connectedSocket = this->_listeningSocket.acceptFirstRequestInQueue();
}
