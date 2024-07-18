/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:31:20 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 13:50:51 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConf.hpp"

HttpConf::HttpConf( void ): AConf(HTTP) {};
HttpConf::~HttpConf( void ) {};
HttpConf::HttpConf( const HttpConf& obj ): AConf(HTTP) {
	this->_type = obj._type;
	this->_server = obj._server;
	this->_settings = obj._settings;
}

const std::vector<ServerConf>& HttpConf::getServer( void ) const { return (_server); };

void HttpConf::addNestedBlock( context type ) {
	if (type == SERVER) {
		ServerConf s(_settings);
		_server.push_back(s);
	}
	else if (type == LOCATION) {
		_server.back().addNestedBlock(type);
	}
}

void HttpConf::setSetting( std::string key, std::string value, context type ) {
	if (type == HTTP)
		_settings[key] = value;
	else
		_server.back().setSetting(key, value, type);
}

/*	Access the servers to check their specific configurations. the behavior it is
	a bit different from the other checkSpecficSettings(), since http class has no
	specific settings to be set.
*/
enum conf_err HttpConf::checkSettings( void ) {
	enum conf_err n = CONF_SUCCESS;
	n = checkSharedSettings();
	if (n) return (n);
	std::vector<ServerConf>::iterator serverIt = _server.begin();
	while (serverIt != _server.end()) {
		n = (*serverIt).checkSettings();
		if (n) return (n);
		serverIt++;
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
