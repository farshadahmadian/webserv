/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConf.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:24:02 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 14:00:06 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ACONF_HPP__
# define __ACONF_HPP__

#include "parser.h"
#include "parse_utils.hpp"

class AConf
{
	protected:
		context _type;
		std::map<std::string, std::string> _settings;

		AConf( std::map<std::string, std::string> settings, context type );
		AConf( context type );
		virtual ~AConf( void ) {};

		void setSharedSettingsDefaults( void );

		virtual void addNestedBlock( context type ) = 0;
		virtual void setSetting( std::string key, std::string value, context active ) = 0;

		virtual enum conf_err checkSettings( void ) = 0;

	public:
		const std::map<std::string, std::string> getSettings( void ) const;
		enum conf_err checkSharedSettings( void );
		static const std::string sharedSettings[N_SHARED_DIR];

		virtual void displaySettings( void ) const = 0;

	private:
		// UN-USED CONSTRUCTORS
		AConf( void );
		AConf& operator=( const AConf& );
};

#endif /* __ACONF_HPP__ */
