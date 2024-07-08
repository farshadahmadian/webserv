/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 11:38:41 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
Location::~Location( void ) {};
Location::Location( const Location& obj ):
	_i(obj._i),
	_settings(obj._settings) {};
Location::Location ( size_t i ): 
	_i(i),
	_settings(std::map<std::string, std::string>()) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
size_t Location::getIndex( void ) { return (_i); };
std::map<std::string, std::string> Location::getSettings( void ) { return (_settings); };

void Location::setSettings( std::string key, std::string value ) { _settings[key] = value; };

// -------------------------------------------------------------------- DISPLAY
void Location::displayLocationSettings( void ) {
	VERBOSE ? std::cout << G("          [LOCATION]") << std::endl : std::cout;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		VERBOSE ? std::cout << "            * " << (*it).first << ": " << (*it).second << std::endl : std::cout;
		it++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
Location::Location ( void ) {/* Not needed */};
void Location::operator=( const Location& ) {/* Not needed */};
