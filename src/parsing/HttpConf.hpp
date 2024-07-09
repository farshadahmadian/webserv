/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:45:52 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 09:54:39 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTPCONF_HPP__
# define __HTTPCONF_HPP__

#include "ServerConf.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <map>
#include "colors.h"

class HttpConf
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		HttpConf( void );
		~HttpConf( void );
		void displayHttpSettings( void ); // -------------------------- DISPLAY

	public: // ------------------------------------------------- PUBLIC MEMBERS
		std::vector<ServerConf>& getServer( void );
		size_t& getServerNumber( void );
		std::map<std::string, std::string> getSettings( void );

		void addServer( void );
		void addLocation( void );
		void setHttpSettings( std::string directive, std::string value );
		void setServerSettings( std::string directive, std::string value );
		void setLocationSettings( std::string directive, std::string value );

	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _n_server;
		std::vector<ServerConf> _server;
		std::map<std::string, std::string> _settings;

		HttpConf( const HttpConf& ); // --------------------------- UNUSED CONSTRUCTORS
		void operator=( const HttpConf& );
};

#endif /* __HTTPCONF_HPP__ */
