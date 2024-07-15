/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 11:17:24 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConf.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
LocationConf::~LocationConf( void ) {};
LocationConf::LocationConf( const LocationConf& obj ):
	_i(obj._i),
	_settings(obj._settings) {};
LocationConf::LocationConf ( size_t i ):
	_i(i),
	_settings(std::map<std::string, std::string>()) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
size_t LocationConf::getIndex( void ) { return (_i); };
std::map<std::string, std::string> LocationConf::getSettings( void ) { return (_settings); };

void LocationConf::setSettings( std::string key, std::string value ) { _settings[key] = value; };

// -------------------------------------------------------------- CONF CHECKING
void LocationConf::checkConfiguration( void ) {
	for (size_t i = 0; i < N_LOCATION_DIR-1; i++) {
		// if (_settings.find(locationDirectives[i]) == _settings.end())
		// 	_settings[locationDirectives[i]] = locationDefaults[i];
	}
}

// ------------------------------------------------------------- DIRECTIVE LIST
const std::string LocationConf::locationDirectives[N_LOCATION_DIR] = {
	"uri",
	"root",
	"index",
	"method",
	"autoindex",
	"cgi"
};
// -------------------------------------------------------------------- DISPLAY
void LocationConf::displayLocationSettings( void ) {
	std::cout << G("          [LOCATION]") << std::endl;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "            * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
LocationConf::LocationConf ( void ) {/* Not needed */};
void LocationConf::operator=( const LocationConf& ) {/* Not needed */};
