/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:33:07 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/09 13:27:01 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONCONF_HPP__
# define __LOCATIONCONF_HPP__

#include "AConf.hpp"
#include "LocationConf.hpp"

class LocationConf: public AConf
{
	public:
		LocationConf( std::map<std::string, std::string> settings );
		~LocationConf( void );
		LocationConf& operator=( const LocationConf& );

		void setDefaults( void );
		void setSetting( std::string key, std::string value, context active );
		void addNestedBlock( context type );

		enum conf_err checkSettings( void );

		void displaySettings( void ) const;
		static const std::string locationSettings[N_LOCATION_DIR];

	private:

		// UN-USED CONSTRUCTORS
		LocationConf( void );
};

#endif /* __LOCATIONCONF_HPP__ */
