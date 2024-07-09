/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 09:59:02 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

// ----------------------------------------------------------- USED CONSTRUCTORS
ServerConf::~ServerConf( void ) {};
ServerConf::ServerConf( const ServerConf& obj ):
	_i(obj._i),
	_n_location(obj._n_location),	// {};
	_location(obj._location),		// was a mistake !!!
	_settings(obj._settings) {};

ServerConf::ServerConf ( size_t i ): 
	_i(i),
	_n_location(0),
	_location(std::vector<LocationConf>()),
	_settings(std::map<std::string, std::string>()) {}; // how initialize the things && how copy

// ------------------------------------------------------------- PUBLIC MEMBERS
size_t ServerConf::getIndex( void ) { return (_i); };
std::map<std::string, std::string> ServerConf::getSettings( void ) { return (_settings); };
std::vector<LocationConf>& ServerConf::getLocation( void ) { return (_location); };

void ServerConf::setSettings( std::string key, std::string value ) { _settings[key] = value; };
void ServerConf::addLocation( void ) {
	LocationConf l(_n_location);
	_location.push_back(l);
	_n_location++;
};

// -------------------------------------------------------------------- DISPLAY
void ServerConf::displayServerSettings( void ) {
	std::cout << G("      [SERVER]") << std::endl;
	std::map<std::string, std::string>::iterator it = _settings.begin();
	while (it != _settings.end()) {
		std::cout << "      * " << (*it).first << ": " << (*it).second << std::endl;
		it++;
	}
	std::vector<LocationConf>::iterator locationIt = _location.begin();
	while (locationIt != _location.end()) {
		(*locationIt).displayLocationSettings();
		locationIt++;
	}
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
ServerConf::ServerConf ( void ) {/* Not needed */};
void ServerConf::operator=( const ServerConf& ) {/* Not needed */};
