/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/20 15:36:24 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

// ---------------------------------------------------------- USED CONSTRUCTORS
Http::Http( void ):
	_n_server(0),
	_server(std::vector<Server>()),
	_settings(std::map<std::string, std::string>()) {};
Http::~Http( void ) {};

// ------------------------------------------------------------- PUBLIC MEMBERS
std::vector<Server>& Http::getServer( void ) { return (_server); };
size_t& Http::getServerNumber( void ) { return (_n_server); };
std::map<std::string, std::string> Http::getSettings( void ) { return (_settings); };

void Http::addServer( void ) {
	VERBOSE ? std::cout << G("* [SERVER CONTEXT ON] server index: ") << _n_server << std::endl : std::cout;
	Server s(_n_server);
	_server.push_back(s);
	_n_server++;
}
void Http::addLocation( void ) { _server.back().addLocation(); }
void Http::setHttpSettings( std::string key, std::string value ) { _settings[key] = value; };
void Http::setServerSettings( std::string key, std::string value ) { _server.back().setSettings(key, value); };
void Http::setLocationSettings( std::string key, std::string value ) { _server.back().getLocation().back().setSettings(key, value); };

// -------------------------------------------------------------------- DISPLAY
void Http::displayHttpSettings( void ) {
	VERBOSE ? std::cout << B("************ { WEB SERVER CONFIGURATION } ************") << std::endl : std::cout;
	VERBOSE ? std::cout << G("[HTTP]") << std::endl : std::cout;
	std::map<std::string, std::string>::iterator httpIt = _settings.begin();
	while (httpIt != _settings.end()) {
		VERBOSE ? std::cout << "  * " << (*httpIt).first << ": " << (*httpIt).second << std::endl : std::cout;
		httpIt++;
	}
	std::vector<Server>::iterator serverIt = getServer().begin();
	while (serverIt != getServer().end()) {
		(*serverIt).displayServerSettings();
		serverIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
Http::Http( const Http& ) {/* Not needed */};
void Http::operator=( const Http& ) {/* Not needed */};
