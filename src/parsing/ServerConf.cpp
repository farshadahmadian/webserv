/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:32:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/10 11:33:32 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( std::map<std::string, std::string> settings ): AConf(settings, SERVER) {
	_inheritedRoot = _settings["root"];
}
ServerConf::~ServerConf ( void ) {};
ServerConf& ServerConf::operator=( const ServerConf& rhs ) {
	if (this != &rhs) {
		this->_type = rhs._type;
		this->_settings = rhs._settings;
		this->_location = rhs._location;
	}
	return (*this);
}

const std::string ServerConf::serverSettings[N_SERVER_DIR] = {
	"server_name",
	"port",
	"cgi"
};

void ServerConf::setDefaults( void ) {
	setSharedSettingsDefaults();
	if (_settings.find("server_name") == _settings.end()) _settings["server_name"] = DEFAULT_SERVER_NAME;
	if (_settings.find("port") == _settings.end()) _settings["port"] = DEFAULT_PORT;
	if (_settings.find("cgi") == _settings.end()) _settings["cgi"] = DEFAULT_CGI;
	std::vector<LocationConf>::iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		(*locationIt).setDefaults();
		locationIt++;
	}
}

const std::vector<LocationConf>& ServerConf::getLocation( void ) const { return (_location); };

void ServerConf::addNestedBlock( context type ) {
	if (type == LOCATION) {
		LocationConf l(_settings);
		_location.push_back(l);
	}
}

void ServerConf::setSetting( std::string key, std::string value, context type ) {
	if (type == SERVER) {
		if (key == "root") {
			if (value[0] == '/' || !value.compare(0 ,2, "./") || !value.compare(0 ,3, "../"))
				_settings[key] = value;
			else
				_settings[key] = _inheritedRoot + "/" + value;
		}
		else
			_settings[key] = value;
	}
	else
		_location.back().setSetting(key, value, type);
}

enum conf_err ServerConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;

	// n = checkSharedSettings();
	// if (n) return (n);

	// // check specific
	// // PORT			"8080"				: set limit?
	// if (!isValidNumber(_settings["port"], "port")) return (E_PORT);
	// // if (!isValidServerName(_settings["host"])) return (E_HOST);

	// // iterate locations
	// std::vector<LocationConf>::iterator locationIt = _location.begin();
	// while (locationIt != _location.end()) {
	// 	n = (*locationIt).checkSettings();
	// 	if (n) return (n);
	// 	locationIt++;
	// }
	return (n);
}

void ServerConf::displaySettings( void ) const {
	std::cout << GREEN << "----- [SERVER]" << RESET << std::endl;
	std::map<std::string, std::string>::const_iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "        * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}

	std::vector<LocationConf>::const_iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		(*locationIt).displaySettings();
		locationIt++;
	}
}
