/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 11:39:57 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConf.hpp"

// ---------------------------------------------------------- USED CONSTRUCTORS
HttpConf::HttpConf( void ):
	_server(std::vector<ServerConf>()),
	_settings(std::map<std::string, std::string>()) {
	setDefaults();
};
HttpConf::~HttpConf( void ) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
std::vector<ServerConf>& HttpConf::getServer( void ) { return (_server); };
std::map<std::string, std::string> HttpConf::getSettings( void ) { return (_settings); };

void HttpConf::addNestedBlock( context type ) {
	if (type == SERVER) {
		ServerConf s(_settings);
		_server.push_back(s);
	}
	else if (type == LOCATION)
		_server.back().addNestedBlock(type);
}

void HttpConf::setDefaults( void ) {
	_settings["keepalive_timeout"] = "75";
	_settings["client_body_buffer_size"] = "80";
	_settings["index"] = "index index.html";
	_settings["autoindex"] = "off";
	_settings["root"] = "./www/html";
};
void HttpConf::setSettings( std::string key, std::string value, context type ) {
	if (type == HTTP)
		_settings[key] = value;
	else
		_server.back().setSettings(key, value, type);
}

// -------------------------------------------------------------- CONF CHECKING
void HttpConf::checkConfiguration( void ) {
	/*
		WHAT TO CHECK ????
	*/
	std::vector<ServerConf>::iterator serverIt = getServer().begin();
	while (serverIt != getServer().end()) {
		(*serverIt).checkConfiguration();
		serverIt++;
	}
}

// ------------------------------------------------------------- DIRECTIVE LIST
const std::string HttpConf::httpDirectives[6] = {
	"keepalive_timeout",
	"client_body_buffer_size",
	"root",
	"index",
	"autoindex",
	"server"
};

// -------------------------------------------------------------------- DISPLAY
void HttpConf::displaySettings( void ) {
	std::cout << G("[HTTP]") << std::endl;
	std::map<std::string, std::string>::iterator httpIt = _settings.begin();
	while (httpIt != _settings.end()) {
		std::cout << "  * " << (*httpIt).first << ": " << (*httpIt).second << std::endl;
		httpIt++;
	}
	std::vector<ServerConf>::iterator serverIt = getServer().begin();
	while (serverIt != getServer().end()) {
		(*serverIt).displaySettings();
		serverIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
HttpConf::HttpConf( const HttpConf& ) {/* Not needed */};
void HttpConf::operator=( const HttpConf& ) {/* Not needed */};
