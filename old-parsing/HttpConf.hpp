/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:45:52 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 12:23:13 by ncasteln         ###   ########.fr       */
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
		// HttpConf( std::map<std::string, std::string> settings );
		HttpConf( void );
		~HttpConf( void );

		std::vector<ServerConf>& getServer( void );
		std::map<std::string, std::string> getSettings( void );

		void addNestedBlock( context type );
		void setDefaults( void );
		void setSettings( std::string directive, std::string value, context type );

		void checkConfiguration( void ); // ------------------------ CONFIGURATION
		static const std::string httpDirectives[N_HTTP_DIR];

		void displaySettings( void ); // -------------------------- DISPLAY
	private: // ----------------------------------------------- PRIVATE MEMBERS
		std::vector<ServerConf> _server;
		std::map<std::string, std::string> _settings;

		HttpConf( const HttpConf& ); // --------------------------- UNUSED CONSTRUCTORS
		void operator=( const HttpConf& );
};

#endif /* __HTTPCONF_HPP__ */
