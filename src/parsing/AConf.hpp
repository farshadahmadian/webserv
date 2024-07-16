/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConf.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:24:02 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/16 15:58:58 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ACONF_HPP__
# define __ACONF_HPP__

#include "parser.h"

class AConf
{
	protected:
		context _type;
		std::map<std::string, std::string> _settings;

		AConf( std::map<std::string, std::string> settings, context type );
		AConf( context type );
		virtual ~AConf( void ) {};

		std::map<std::string, std::string> getSettings( void );
		void setSharedSettingsDefaults( void );

		virtual void addNestedBlock( context type ) = 0;
		virtual void checkConfiguration( void ) = 0;
		virtual void setSetting( std::string key, std::string value, context active ) = 0;

	public:
		virtual void displaySettings( void ) const = 0;
		static const std::string sharedDirectives[N_SHARED_DIR];

	private:
		// UN-USED CONSTRUCTORS
		AConf( void );
		AConf& operator=( const AConf& );
};

#endif /* __ACONF_HPP__ */
