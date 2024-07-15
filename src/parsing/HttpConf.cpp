/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 11:45:51 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConf.hpp"

// ---------------------------------------------------------- USED CONSTRUCTORS
HttpConf::HttpConf( void ):
	_n_server(0),
	_server(std::vector<ServerConf>()),
	_settings(std::map<std::string, std::string>()) {};
HttpConf::~HttpConf( void ) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
std::vector<ServerConf>& HttpConf::getServer( void ) { return (_server); };
size_t& HttpConf::getServerNumber( void ) { return (_n_server); };
std::map<std::string, std::string> HttpConf::getSettings( void ) { return (_settings); };

void HttpConf::addServer( void ) {
	ServerConf s(_n_server);
	_server.push_back(s);
	_n_server++;
}
void HttpConf::addLocation( void ) { _server.back().addLocation(); }
void HttpConf::setDefaults( void ) {
	_settings["keepalive_timeout"] = "75";
	_settings["client_body_buffer_size"] = "80";
	_settings["index"] = "index index.html";
	_settings["autoindex"] = "off";
	_settings["root"] = "./www/html";
};
void HttpConf::setHttpSettings( std::string key, std::string value ) { _settings[key] = value; };
void HttpConf::setServerSettings( std::string key, std::string value ) { _server.back().setSettings(key, value); };
void HttpConf::setLocationSettings( std::string key, std::string value ) { _server.back().getLocation().back().setSettings(key, value); };

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
const std::string HttpConf::httpDirectives[N_HTTP_DIR] = {
	"keepalive_timeout",
	"client_body_buffer_size",
	"root",
	"index",
	"autoindex",
	"server"
};

// -------------------------------------------------------------------- DISPLAY
void HttpConf::displayHttpSettings( void ) {
	std::cout << G("[HTTP]") << std::endl;
	std::map<std::string, std::string>::iterator httpIt = _settings.begin();
	while (httpIt != _settings.end()) {
		std::cout << "  * " << (*httpIt).first << ": " << (*httpIt).second << std::endl;
		httpIt++;
	}
	std::vector<ServerConf>::iterator serverIt = getServer().begin();
	while (serverIt != getServer().end()) {
		(*serverIt).displayServerSettings();
		serverIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
HttpConf::HttpConf( const HttpConf& ) {/* Not needed */};
void HttpConf::operator=( const HttpConf& ) {/* Not needed */};
