/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:31:20 by ncasteln          #+#    #+#             */
/*   Updated: 2024/09/09 13:54:03 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConf.hpp"
#include "Exception.hpp"
#include <cstdlib>
#include <algorithm>

HttpConf::HttpConf( void ): AConf(HTTP) {
	_inheritedRoot = DEFAULT_ROOT;
	_settings["root"] = _inheritedRoot;
};
HttpConf::~HttpConf( void ) {};
HttpConf::HttpConf( const HttpConf& obj ): AConf(HTTP) {
	this->_type = obj._type;
	this->_server = obj._server;
	this->_settings = obj._settings;
	this->_inheritedRoot = obj._inheritedRoot;
}
HttpConf& HttpConf::operator=( const HttpConf& rhs ) {
	if (this != &rhs) {
		this->_type = rhs._type;
		this->_server = rhs._server;
		this->_settings = rhs._settings;
		this->_inheritedRoot = rhs._inheritedRoot;
	}
	return (*this);
}

const std::vector<ServerConf>& HttpConf::getServer( void ) const { return (_server); };

void HttpConf::addNestedBlock( context type ) {
	if (type == SERVER) {
		ServerConf s(_settings);
		_server.push_back(s);
	}
	else if (type == LOCATION)
		_server.back().addNestedBlock(type);
}

void HttpConf::setSetting( std::string key, std::string value, context type ) {
	if (type == HTTP) {
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
		_server.back().setSetting(key, value, type);
}

void HttpConf::setDefaults( void ) {
	setSharedSettingsDefaults();
	std::vector<ServerConf>::iterator serverIt = _server.begin();
	while (serverIt != _server.end()) {
		(*serverIt).setDefaults();
		serverIt++;
	}
}

/*	Access the servers to check their specific configurations. the behavior it is
	a bit different from the other checkSpecficSettings(), since http class has no
	specific settings to be set.
*/
enum conf_err HttpConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;
	std::vector<int> ports;

	n = checkSharedSettings();
	if (n) return (n);
	std::vector<ServerConf>::iterator serverIt = _server.begin();
	while (serverIt != _server.end()) {
		ports.push_back(std::atol((*serverIt).getPort().c_str()));
		n = (*serverIt).checkSettings();
		if (n) return (n);
		serverIt++;
	}

	std::sort(ports.begin(), ports.end());
	for (size_t i = 1; i < ports.size(); ++i) {
		if (ports[i] == ports[i - 1]) {
			n = E_DOUBLEPORT;
		}
	}
	return (n);
}

void HttpConf::displaySettings( void ) const {
	std::cout << GREEN << "[HTTP]" << RESET << std::endl;
	std::map<std::string, std::string>::const_iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "  * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}

	std::vector<ServerConf>::const_iterator serverIt = _server.begin();
	while (serverIt != _server.end()) {
		(*serverIt).displaySettings();
		serverIt++;
	}
}
