/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:26:25 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 12:12:08 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.hpp"

Exception::Exception(void) : std::exception(), _errMsg(""), _error(NO_ERROR) {
	return;
}

Exception::~Exception(void) throw(){
	return;
}

Exception::Exception(std::string errMsg, enum server_err error) : std::exception(), _errMsg(errMsg) , _error(error) {
	return;
}

char const *Exception::what() const throw() {
	if (_errMsg.empty())
		return "Exception\n";
	return this->_errMsg.c_str();
}

enum server_err Exception::getError(void) const{
	return this->_error;
}

void Exception::setError(enum server_err error) {
	this->_error = error;
	return;
}
