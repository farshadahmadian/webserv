/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:32:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/05 11:36:24 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( std::map<std::string, std::string> settings ): AConf(settings, SERVER) {
	setSpecificSettingsDefaults();
}
ServerConf::~ServerConf ( void ) {
	setSpecificSettingsDefaults();
};
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

void ServerConf::setSpecificSettingsDefaults( void ) {
	_settings["server_name"] = DEFAULT_SERVER_NAME;
	_settings["port"] = DEFAULT_PORT;
}

const std::vector<LocationConf>& ServerConf::getLocation( void ) const { return (_location); };

void ServerConf::addNestedBlock( context type ) {
	if (type == LOCATION) {
		LocationConf l(_settings);
		_location.push_back(l);
	}
}

void ServerConf::setSetting( std::string key, std::string value, context type ) {
	if (type == SERVER)
		_settings[key] = value;
	else
		_location.back().setSetting(key, value, type);
}

enum conf_err ServerConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;

	n = checkSharedSettings();
	if (n) return (n);

	// check specific
	// PORT			"8080"				: set limit?
	if (!isValidNumber(_settings["port"], "port")) return (E_PORT);
	// if (!isValidServerName(_settings["host"])) return (E_HOST);

	// iterate locations
	std::vector<LocationConf>::iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		n = (*locationIt).checkSettings();
		if (n) return (n);
		locationIt++;
	}
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
