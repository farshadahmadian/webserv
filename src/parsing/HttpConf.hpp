/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:45:52 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 12:24:44 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTPCONF_HPP__
# define __HTTPCONF_HPP__

#include "ServerConf.hpp"
#include <stdexcept>
#include <fstream>

class HttpConf
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		HttpConf( void );
		~HttpConf( void );
		void displayHttpSettings( void ); // -------------------------- DISPLAY

		std::vector<ServerConf>& getServer( void );
		size_t& getServerNumber( void );
		std::map<std::string, std::string> getSettings( void );

		void addServer( void );
		void addLocation( void );
		void setDefaults( void );
		// void setSettings( std::string directive, std::string value );
		void setHttpSettings( std::string directive, std::string value );
		void setServerSettings( std::string directive, std::string value );
		void setLocationSettings( std::string directive, std::string value );

		void checkConfiguration( void ); // ------------------------ CONFIGURATION
		static const std::string httpDirectives[N_HTTP_DIR];
	private: // ----------------------------------------------- PRIVATE MEMBERS
		// block _type;
		size_t _n_server;
		std::vector<ServerConf> _server;
		std::map<std::string, std::string> _settings;

		HttpConf( const HttpConf& ); // --------------------------- UNUSED CONSTRUCTORS
		void operator=( const HttpConf& );
};

#endif /* __HTTPCONF_HPP__ */
