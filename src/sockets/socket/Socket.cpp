/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:26:30 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/19 15:05:54 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : _socketFd(0) {
	return;
}

Socket::~Socket(void) {
	close(this->_socketFd);
	return;
}

int Socket::getSocketFd(void) const {
	return this->_socketFd;
}

void Socket::setSocketFd(int socketFd) {
	this->_socketFd = socketFd;
}
