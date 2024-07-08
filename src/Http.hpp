/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 11:38:58 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTP_HPP__
# define __HTTP_HPP__

#include "Server.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <map>
#include "colors.h"

class Http
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Http( void );
		~Http( void );
		void displayHttpSettings( void ); // -------------------------- DISPLAY

	public: // ------------------------------------------------- PUBLIC MEMBERS
		std::vector<Server>& getServer( void );
		size_t& getServerNumber( void );
		std::map<std::string, std::string> getSettings( void );

		void addServer( void );
		void addLocation( void );
		void setHttpSettings( std::string directive, std::string value );
		void setServerSettings( std::string directive, std::string value );
		void setLocationSettings( std::string directive, std::string value );
		

		// // temp
		// void ourMain( void ) {
		// 	std::vector<Server>::iterator it = _server.begin();
		// 	while (it != _server.end()) {
		// 		_server.startServer();
		// 		it++;
		// 	}
		// };

	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _n_server;
		std::vector<Server> _server;
		std::map<std::string, std::string> _settings;

		Http( const Http& ); // --------------------------- UNUSED CONSTRUCTORS
		void operator=( const Http& );
};

#endif /* __HTTP_HPP__ */
