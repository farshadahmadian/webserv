/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/21 15:16:09 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <iostream>
#include <vector>
#include <map>
#include "Location.hpp"
#include "colors.h"

class Server
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Server( size_t );
		~Server( void );
		Server( const Server& );
		
	public: // ------------------------------------------------- PUBLIC MEMBERS
		size_t getIndex( void );
		std::vector<Location>& getLocation( void );
		std::map<std::string, std::string> getSettings( void ); /////// ?????? need to return reference ???????????
		void setSettings( std::string key, std::string value );
		void addLocation( void );

		void displayServerSettings( void ); // ------------------------ DISPLAY

		// temp
		// void startServer( void ) {
			//
		// }
	
	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _i;
		size_t _n_location;
		std::vector<Location> _location;
		std::map<std::string, std::string> _settings;
		
		Server( void ); // -------------------------------- UNUSED CONSTRUCTORS
		void operator=( const Server& );
};

#endif /* __SERVER_HPP__ */
