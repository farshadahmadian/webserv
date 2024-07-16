/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 11:38:16 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
ServerConf::~ServerConf( void ) {};
ServerConf::ServerConf( const ServerConf& obj ): // change somthing? ?? ? ?? ? ?
	_location(obj._location),		// was a mistake !!!
	_settings(obj._settings) {
};

ServerConf::ServerConf( std::map<std::string, std::string> settings ):
	_location(std::vector<LocationConf>()),
	_settings(settings) { // DEEP COPIED ???????????
	// setDefaults(); --- set defaults of server ?
}; // how initialize the things && how copy

// ------------------------------------------------------------- PUBLIC MEMBERS
std::map<std::string, std::string> ServerConf::getSettings( void ) { return (_settings); };
std::vector<LocationConf>& ServerConf::getLocation( void ) { return (_location); };

void ServerConf::setDefaults( void ) {
	_settings["server_name"] = "hello.com";
	_settings["port"] = "8080";
	_settings["host"] = "127.0.0.1";
};
void ServerConf::setSettings( std::string key, std::string value, context type ) {
	if (type == SERVER)
		_settings[key] = value;
	else
		_location.back().setSettings(key, value, type);
};

void ServerConf::addNestedBlock( context type ) {
	if (type == LOCATION) {
		LocationConf l(_settings);
		_location.push_back(l);
	}
}

// -------------------------------------------------------------- CONF CHECKING
void ServerConf::checkConfiguration( void ) {
	// for (size_t i = 0; i < N_SERVER_DIR-1; i++) {
	// 	if (_settings.find(serverDirectives[i]) == _settings.end())
	// 		_settings[serverDirectives[i]] = serverDefaults[i];
	// }
	if (_settings.find("host") == _settings.end()) {
		// throw
	}
	if (_settings.find("root") == _settings.end()) {
		// throw
	}
	if (_settings.find("port") == _settings.end()) {
		// throw
	}
	std::vector<LocationConf>::iterator locationIt = getLocation().begin();
	while (locationIt != getLocation().end()) {
		(*locationIt).checkConfiguration();
		locationIt++;
	}
}

// ------------------------------------------------------------- DIRECTIVE LIST
const std::string ServerConf::serverDirectives[N_SERVER_DIR] = {
	"host",
	"server_name",
	"port", //
	"keepalive_timeout",
	"client_body_buffer_size",
	"root",
	"index",
	"location"
};

// -------------------------------------------------------------------- DISPLAY
void ServerConf::displaySettings( void ) {
	std::cout << G("      [SERVER]") << std::endl;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "      * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
	std::vector<LocationConf>::iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		(*locationIt).displaySettings();
		locationIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
ServerConf::ServerConf ( void ) {/* Not needed */};
void ServerConf::operator=( const ServerConf& ) {/* Not needed */};
