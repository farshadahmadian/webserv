/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:26:30 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/04 17:55:30 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : _socketFd(0) {
	return;
}

Socket::Socket(int socketFd) : _socketFd(socketFd) {
	return;
}

Socket::Socket(Socket const &other) : _socketFd(other._socketFd) {
	return;
}

Socket &Socket::operator=(Socket const &rhs) {
	if (this != &rhs)
		this->_socketFd = rhs._socketFd;
	return *this;
}

Socket::~Socket(void) {
	// close(this->_socketFd);
	return;
}

int Socket::getSocketFd(void) const {
	return this->_socketFd;
}

void Socket::setSocketFd(int socketFd) {
	this->_socketFd = socketFd;
}
