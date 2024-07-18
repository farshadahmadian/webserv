/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:34:11 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 13:55:15 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConf.hpp"

LocationConf::LocationConf( std::map<std::string, std::string> settings ): AConf(settings, LOCATION) {}
LocationConf::~LocationConf ( void ) {};
LocationConf& LocationConf::operator=( const LocationConf& rhs ) {
	if (this != &rhs) {
		this->_type = rhs._type;
		this->_settings = rhs._settings;
	}
	return (*this);
}

const std::string LocationConf::locationSettings[N_LOCATION_DIR] = {
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



enum conf_err LocationConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;

	// check sshared
	n = checkSharedSettings();

	// check specific
	// URI				""					: still to decide						-
	// CGI				""					: still to decide						-
	if (!isValidMethod(_settings["method"])) return (E_METHOD);

	return (n);
}

void LocationConf::displaySettings( void ) const {
	std::cout << GREEN << "--------- [LOCATION]" << RESET << std::endl;
	std::map<std::string, std::string>::const_iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "             * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
}
