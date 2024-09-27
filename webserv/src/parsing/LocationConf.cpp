/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:34:11 by ncasteln          #+#    #+#             */
/*   Updated: 2024/09/09 13:54:52 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConf.hpp"

LocationConf::LocationConf( std::map<std::string, std::string> settings ): AConf(settings, LOCATION) {
	_inheritedRoot = settings["root"];
}
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
	"cgi",
	"redirect",
	"index",
};

void LocationConf::setDefaults( void ) {
	setSharedSettingsDefaults();
	if (_settings.find("uri") == _settings.end()) _settings["uri"] = DEFAULT_URI;
	if (_settings.find("method") == _settings.end()) _settings["method"] = DEFAULT_METHOD;
	if (_settings.find("cgi") == _settings.end()) _settings["cgi"] = DEFAULT_CGI;
	if (_settings.find("redirect") == _settings.end()) _settings["redirect"] = DEFAULT_REDIRECT;
	if (_settings.find("index") == _settings.end()) _settings["index"] = DEFAULT_INDEX;
};

void LocationConf::addNestedBlock( context type ) { (void)type; /* not implemented */}

void LocationConf::setSetting( std::string key, std::string value, context type ) {
	if (type == LOCATION) {
		if (key == "root") {
			if (value[0] == '/' || !value.compare(0 ,2, "./") || !value.compare(0 ,3, "../"))
				_settings[key] = value;
			else
				_settings[key] = _inheritedRoot + "/" + value;
		}
		else
			_settings[key] = value;
	}
}

enum conf_err LocationConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;

	// check shared
	n = checkSharedSettings();

	// // check specific
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
	std::cout << std::endl;
}

std::string LocationConf::getASettingValue(std::string key) {
	if (this->_settings.find(key) != this->_settings.end()) {
		return this->_settings[key];
	}
	return "";
}
