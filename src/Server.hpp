/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/19 17:01:28 by ncasteln         ###   ########.fr       */
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
	public: // -------------------------------------------- PUBLIC CONSTRUCTORS
		Server( size_t );
		~Server( void );
		Server( const Server& );
	private:  // ----------------------------------------- PRIVATE CONSTRUCTORS
		Server( void );
		void operator=( const Server& );
		
	public: // ------------------------------------------------- PUBLIC MEMBERS
		size_t getIndex( void );
		std::vector<Location> getLocation( void );
		std::map<std::string, std::string> getSettings( void );
		void setSettings( std::string key, std::string value );
		void addLocation( void );

		void displayServerSettings( void );
	
	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _i;
		size_t _n_location;
		std::vector<Location> _location;
		std::map<std::string, std::string> _settings;
};

#endif /* __SERVER_HPP__ */
