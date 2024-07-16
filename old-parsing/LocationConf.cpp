/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 09:01:47 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConf.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
LocationConf::~LocationConf( void ) {};
LocationConf::LocationConf( const LocationConf& obj ):
	_settings(obj._settings) {};
LocationConf::LocationConf ( std::map<std::string, std::string> settings ):
	_settings(settings) {
	// need to copy only some properties
	// need to copy only some properties
	// need to copy only some properties
	// need to copy only some properties
	// need to copy only some properties
	// need to copy only some properties
};

// ------------------------------------------------------------- PUBLIC MEMBERS
std::map<std::string, std::string> LocationConf::getSettings( void ) { return (_settings); };

void LocationConf::setSettings( std::string key, std::string value, context type ) {
	if (type == LOCATION) _settings[key] = value;
};

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
	"method",
	"cgi", //
	"keepalive_timeout",
	"client_body_buffer_size",
	"root",
	"index",
	"autoindex"
};

// -------------------------------------------------------------------- DISPLAY
void LocationConf::displaySettings( void ) {
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
