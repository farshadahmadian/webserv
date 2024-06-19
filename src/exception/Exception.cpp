/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:26:25 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/19 14:22:56 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.hpp"

Exception::Exception(void) : std::exception(), _errMsg(""), _error(NO_ERROR) {
	return;
}

Exception::~Exception(void) throw(){
	return;
}

Exception::Exception(std::string errMsg, t_error error) : std::exception(), _errMsg(errMsg) , _error(error) {
	return;
}

char const *Exception::what() const throw() {
	if (_errMsg.empty())
		return "Exception\n";
	return this->_errMsg.c_str();
}

t_error Exception::getError(void) const{
	return this->_error;
}

void Exception::setError(t_error error) {
	this->_error = error;
	return;
}
