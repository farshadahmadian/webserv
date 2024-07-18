/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:32:58 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 09:26:28 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONCONF_HPP__
# define __LOCATIONCONF_HPP__

#include "../../include/parser.h"

class LocationConf
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		LocationConf( std::map<std::string, std::string> );
		~LocationConf( void );
		LocationConf( const LocationConf& );

		// std::vector<LocationConf>& getNothing( void );
		std::map<std::string, std::string> getSettings( void );

		// void addNestedBlock( context type );
		// void setDefaults( void );
		void setSettings( std::string key, std::string value, context type );

		void checkConfiguration( void ); // --------------------- CONFIGURATION
		static const std::string locationDirectives[N_LOCATION_DIR];

		void displaySettings( void );  // --------------------- DISPLAY
	private: // ----------------------------------------------- PRIVATE MEMBERS
		// std::vector<LocationConf> _nothing;
		std::map<std::string, std::string> _settings;

		LocationConf( void ); // ------------------------------ UNUSED CONSTRUCTORS
		void operator=( const LocationConf& );
};

#endif /* __LOCATIONCONF_HPP__ */
