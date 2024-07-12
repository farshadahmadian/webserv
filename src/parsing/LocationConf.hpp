/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:32:58 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/12 15:48:52 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONCONF_HPP__
# define __LOCATIONCONF_HPP__

#include "../../include/parser.h"

class LocationConf
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		LocationConf( size_t );
		~LocationConf( void );
		LocationConf( const LocationConf& );
	
		size_t getIndex( void );
		std::map<std::string, std::string> getSettings( void );
		void setSettings( std::string key, std::string value );
		void displayLocationSettings( void );  // --------------------- DISPLAY

		void checkConfiguration( void ); // --------------------- CONFIGURATION
		static const std::string locationDirectives[N_LOCATION_DIR];
		static const std::string locationDefaults[N_LOCATION_DIR];
	
	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _i;
		std::map<std::string, std::string> _settings;

		LocationConf( void ); // ------------------------------ UNUSED CONSTRUCTORS
		void operator=( const LocationConf& );
};

#endif /* __LOCATIONCONF_HPP__ */
