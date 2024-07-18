/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:28:32 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 13:48:45 by ncasteln         ###   ########.fr       */
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
		HttpConf( const HttpConf& );

		void setSetting( std::string key, std::string value, context active);
		void addNestedBlock( context type );
		const std::vector<ServerConf>& getServer( void ) const;

		enum conf_err checkSettings( void );

		void displaySettings( void ) const;

	private:
		std::vector<ServerConf> _server;

		// UN-USED CONSTRUCTORS
		HttpConf& operator=( const HttpConf& );
};

#endif /* __HTTPCONF_HPP__ */
