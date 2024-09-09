/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:35:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/09/09 13:55:08 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_utils.hpp"

#include "parse_utils.hpp"
#include <cstdlib>

std::string& ltrim( std::string& s, const char* to_trim ) {
	size_t pos = s.find_first_not_of(to_trim);
	s.erase(0, pos);
	return (s);
}

std::string& rtrim( std::string& s, const char* to_trim ) {
	size_t pos = s.find_last_not_of(to_trim);
	s.erase(pos + 1);
	return (s);
}

std::string& trim( std::string& s, const char* to_trim ) {
	return (ltrim(rtrim(s, to_trim), to_trim));
}

bool isValidNumber( std::string key, std::string value ) {
	if (key == "keepalive_timeout" || key == "client_body_buffer_size" || key == "port") {
		if (value.find_first_not_of("1234567890") != std::string::npos) {
			return (false);
		}
	}
	return (true);
}

bool isValidMethod( std::string s ) {
	std::string method;
	size_t space_pos;

	while (s.find_first_of(" \t\v\f\r") != std::string::npos) {
		space_pos = s.find_first_of(" \t\v\f\r");
		method = s.substr(0, space_pos);
		if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD")
			return (false);
		s.erase(0, space_pos + 1);
		ltrim(s, " \t\v\f\r");
	}
	if (s != "GET" && s != "POST" && s != "DELETE" && s != "HEAD")
		return (false);
	return (true);
}

bool isValidPort( std::string port ) {
	long portNum = std::atol(port.c_str());
	if (portNum < 1024 || portNum > 65535)
		return (false);
	return (true);
}
