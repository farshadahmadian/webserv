/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:26:30 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/18 11:49:38 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::SocketException::SocketException(void) : std::exception(), _errMsg("") {
	return;
}

Socket::SocketException::~SocketException(void) throw(){
	return;
}

Socket::SocketException::SocketException(std::string errMsg) : std::exception(), _errMsg(errMsg) , _error(NO_ERROR) {
	return;
}

char const *Socket::SocketException::what() const throw() {
	if (_errMsg.empty())
		return "Socket creation failed\n";
	return this->_errMsg.c_str();
}


Socket::Socket(void) {
	return;
}

Socket::~Socket(void) {
	return;
}