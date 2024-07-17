/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:28:32 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/17 10:34:41 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTPCONF_HPP__
# define __HTTPCONF_HPP__

#include "AConf.hpp"
#include "ServerConf.hpp"

class HttpConf: public AConf
{
	public:
		HttpConf( void );
		~HttpConf( void );

		void setSetting( std::string key, std::string value, context active);
		void addNestedBlock( context type );
		const std::vector<ServerConf>& getServer( void ) const;

		enum conf_err checkSettings( void );

		void displaySettings( void ) const;

	private:
		std::vector<ServerConf> _server;

		// UN-USED CONSTRUCTORS
		HttpConf( const HttpConf& );
		HttpConf& operator=( const HttpConf& );
};

#endif /* __HTTPCONF_HPP__ */
