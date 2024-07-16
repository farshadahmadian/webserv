/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 09:03:46 by ncasteln         ###   ########.fr       */
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
		ServerConf( std::map<std::string, std::string> settings );
		~ServerConf( void );  // -------------------------------- USED CONSTRUCTORS
		ServerConf( const ServerConf& );

		std::vector<LocationConf>& getLocation( void );
		std::map<std::string, std::string> getSettings( void );

		void addNestedBlock( context type );
		void setDefaults( void );
		void setSettings( std::string key, std::string value, context type );

		void checkConfiguration( void ); // ------------------------ CONFIGURATION
		static const std::string serverDirectives[N_SERVER_DIR];

		void displaySettings( void ); // ------------------------ DISPLAY
	private:
		std::vector<LocationConf> _location;
		std::map<std::string, std::string> _settings;

		ServerConf( void ); // -------------------------------- UNUSED CONSTRUCTORS
		void operator=( const ServerConf& );
};

#endif /* __SERVERCONF_HPP__ */
