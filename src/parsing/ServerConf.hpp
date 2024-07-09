/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 12:24:11 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERCONF_HPP__
# define __SERVERCONF_HPP__

#include <iostream>
#include <vector>
#include "LocationConf.hpp"

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
		std::map<std::string, std::string> getSettings( void ); /////// ?????? need to return reference ???????????
		void setSettings( std::string key, std::string value );
		void addLocation( void );
		void displayServerSettings( void ); // ------------------------ DISPLAY
		
	private:
		ServerConf( void ); // -------------------------------- UNUSED CONSTRUCTORS
		void operator=( const ServerConf& );
};

#endif /* __SERVERCONF_HPP__ */
