/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 11:44:29 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
Server::~Server( void ) {};
Server::Server( const Server& obj ):
	_i(obj._i),
	_n_location(obj._n_location),	// {};
	_location(obj._location),		// was a mistake !!!
	_settings(obj._settings) {};

Server::Server ( size_t i ): 
	_i(i),
	_n_location(0),
	_location(std::vector<Location>()),
	_settings(std::map<std::string, std::string>()) {}; // how initialize the things && how copy

// ------------------------------------------------------------- PUBLIC MEMBERS
size_t Server::getIndex( void ) { return (_i); };
std::map<std::string, std::string> Server::getSettings( void ) { return (_settings); };
std::vector<Location>& Server::getLocation( void ) { return (_location); };

void Server::setSettings( std::string key, std::string value ) { _settings[key] = value; };
void Server::addLocation( void ) {
	Location l(_n_location);
	_location.push_back(l);
	_n_location++;
};

// -------------------------------------------------------------------- DISPLAY
void Server::displayServerSettings( void ) {
	VERBOSE ? std::cout << G("      [SERVER]") << std::endl : std::cout;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		VERBOSE ? std::cout << "      * " << (*it).first << ": " << (*it).second << std::endl : std::cout;
		it++;
	}
	std::vector<Location>::iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		(*locationIt).displayLocationSettings();
		locationIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
Server::Server ( void ) {/* Not needed */};
void Server::operator=( const Server& ) {/* Not needed */};
