/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:32:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 15:43:23 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( std::map<std::string, std::string> settings ): AConf(settings, SERVER) {
	setSpecificSettingsDefaults();
}
ServerConf::~ServerConf ( void ) {
	setSpecificSettingsDefaults();
};

const std::string ServerConf::serverDirectives[N_SERVER_DIR] = {
	"host",
	"server_name",
	"port"
};

void ServerConf::setSpecificSettingsDefaults( void ) {
	_settings["host"] = DEFAULT_HOST;
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

void ServerConf::checkConfiguration( void ) {
	// for (size_t i = 0; i < N_SERVER_DIR-1; i++) {
	// 	if (_settings.find(serverDirectives[i]) == _settings.end())
	// 		_settings[serverDirectives[i]] = serverDefaults[i];
	// }
	// if (_settings.find("host") == _settings.end()) {
	// 	// throw
	// }
	// if (_settings.find("root") == _settings.end()) {
	// 	// throw
	// }
	// if (_settings.find("port") == _settings.end()) {
	// 	// throw
	// }
	// std::vector<LocationConf>::iterator locationIt = getLocation().begin();
	// while (locationIt != getLocation().end()) {
	// 	(*locationIt).checkConfiguration();
	// 	locationIt++;
	// }
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
