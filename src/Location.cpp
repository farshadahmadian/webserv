/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/19 17:05:20 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// ------------------------------------------------------------------ CANONICAL
Location::Location ( void ) {};
Location::~Location( void ) {};
Location::Location( const Location& obj ):
	_i(obj._i),
	_settings(obj._settings) {};
void Location::operator=( const Location& rhs ) { (void)rhs;/* Not implemented */};

// ---------------------------------------------------------- PARAM CONSTRUCTOR
Location::Location ( size_t i ): 
	_i(i),
	_settings(std::map<std::string, std::string>()) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
size_t Location::getIndex( void ) { return (_i); };
std::map<std::string, std::string> Location::getSettings( void ) { return (_settings); };

void Location::setSettings( std::string key, std::string value ) { _settings[key] = value; };

// -------------------------------------------------------------------- DISPLAY
void Location::displayLocationSettings( void ) {
	std::cout << G("      [LOCATION]") << std::endl;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "        * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
}
