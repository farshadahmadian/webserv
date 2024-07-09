/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:32:58 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/09 16:48:06 by nnabaeei         ###   ########.fr       */
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
	
	public: // ------------------------------------------------- PUBLIC MEMBERS
		size_t getIndex( void );
		std::map<std::string, std::string> getSettings( void );
		void setSettings( std::string key, std::string value );
		void displayLocationSettings( void );  // --------------------- DISPLAY

	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _i;
		std::map<std::string, std::string> _settings;

		LocationConf( void ); // ------------------------------ UNUSED CONSTRUCTORS
		void operator=( const LocationConf& );
};

#endif /* __LOCATIONCONF_HPP__ */
