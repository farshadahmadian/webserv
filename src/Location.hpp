/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:32:58 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/19 16:55:01 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

#include <string>
#include <map>
#include <iostream>
#include "colors.h"

class Location
{
	public: // -------------------------------------------- PUBLIC CONSTRUCTORS
		Location( size_t );
		~Location( void );
		Location( const Location& );
	private:  // ----------------------------------------- PRIVATE CONSTRUCTORS
		Location( void );
		void operator=( const Location& );
		
	public: // ------------------------------------------------- PUBLIC MEMBERS
		size_t getIndex( void );
		std::map<std::string, std::string> getSettings( void );
		void setSettings( std::string key, std::string value );
		void displayLocationSettings( void );
	
	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _i;
		std::map<std::string, std::string> _settings;
};

#endif /* __LOCATION_HPP__ */
