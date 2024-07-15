/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 12:23:08 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERCONF_HPP__
# define __SERVERCONF_HPP__

#include <iostream>
#include <vector>
#include "LocationConf.hpp"
#include "errors.h"

class ServerConf
{
	public:
		ServerConf( size_t ); // -------------------------------- USED CONSTRUCTORS
		~ServerConf( void );
		ServerConf( const ServerConf& );

		size_t _i; // ------------------------------------------------ SETTINGS
		size_t _n_location;
		std::vector<LocationConf> _location;
		std::map<std::string, std::string> _settings;
		size_t getIndex( void );
		std::vector<LocationConf>& getLocation( void );
		std::map<std::string, std::string> getSettings( void );
		void setDefaults( void );
		void setSettings( std::string key, std::string value );
		void addLocation( void );
		void displayServerSettings( void ); // ------------------------ DISPLAY

		void checkConfiguration( void ); // ------------------------ CONFIGURATION
		static const std::string serverDirectives[N_SERVER_DIR];
	private:
		ServerConf( void ); // -------------------------------- UNUSED CONSTRUCTORS
		void operator=( const ServerConf& );
};

#endif /* __SERVERCONF_HPP__ */
