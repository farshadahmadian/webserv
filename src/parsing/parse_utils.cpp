/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:35:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/09 13:04:08 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_utils.hpp"

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

bool isValidServerName( std::string value ) { // extract in utils
	size_t n_dots = 0;
	(void) n_dots;
	(void) value;
	// std::cout << "IP: " << "[" << value << "]" << std::endl;
	// while (!value.empty()) {
	// 	std::cout << "* N dots: " << n_dots << std::endl;
	// 	size_t dot_position = value.find_first_of('.');
	// 	if (dot_position == std::string::npos) break;
	// 	n_dots++;
	// 	std::cout << "dot_position: " << dot_position << std::endl;

	// 	// check from 0 to dot_position
	// 	while
	// 	value.erase(0);

	// 	std::cout << "DEBUG" << std::endl;

	// 	break ; // remove !!!!!!!!!!!!!!!!!!!!!!
	// }
	// if (n_dots != 3)
	// 	return (false);
	return (true);
}

bool isValidNumber( std::string key, std::string value ) { // extract into utils
	if (key == "keepalive_timeout" || key == "client_body_buffer_size" || key == "port") {
		if (value.find_first_not_of("1234567890") != std::string::npos)
			return (false);
		// limits
	}
	if (key == "port") {
		// limits
	}
	return (true);
}


bool isValidMethod( std::string s ) {
	std::string method;
	size_t space_pos;

	while (s.find_first_of(" \t\v\f\r") != std::string::npos) {
		space_pos = s.find_first_of(" \t\v\f\r");
		method = s.substr(0, space_pos);
		if (method != "GET" && method != "POST" && method != "DELETE")
			return (false);
		s.erase(0, space_pos + 1);
		ltrim(s, " \t\v\f\r");
	}
	if (s != "GET" && s != "POST" && s != "DELETE")
		return (false);
	return (true);
}
