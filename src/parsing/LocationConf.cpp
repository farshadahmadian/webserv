/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:34:11 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 16:01:42 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConf.hpp"

LocationConf::LocationConf( std::map<std::string, std::string> settings ): AConf(settings, LOCATION) {}
LocationConf::~LocationConf ( void ) {};

const std::string LocationConf::locationDirectives[N_LOCATION_DIR] = {
	"uri",
	"method",
	"cgi"
};

void LocationConf::setSpecificSettingsDefaults( void ) {
	_settings["uri"] = DEFAULT_URI;
	_settings["method"] = DEFAULT_METHOD;
	_settings["cgi"] = DEFAULT_CGI;
};


void LocationConf::addNestedBlock( context type ) { (void)type; /* not implemented */}

void LocationConf::setSetting( std::string key, std::string value, context type ) {
	if (type == LOCATION)
		_settings[key] = value;
}

void LocationConf::checkConfiguration( void ) {
	// for (size_t i = 0; i < N_LOCATION_DIR-1; i++) {
	// 	// if (_settings.find(locationDirectives[i]) == _settings.end())
	// 	// 	_settings[locationDirectives[i]] = locationDefaults[i];
	// }
}

void LocationConf::displaySettings( void ) const {
	std::cout << GREEN << "--------- [LOCATION]" << RESET << std::endl;
	std::map<std::string, std::string>::const_iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "             * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
}
