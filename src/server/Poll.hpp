/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:54:45 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/21 10:09:25 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __POLL_HPP__
# define __POLL_HPP__

#include "server.h"
#include "Parser.hpp"

class Poll
{
	public:
		Poll( const Parser& configuration );
		~Poll( void );

		void init( void );
		void start( void );

	private:
		std::vector<Server> _serverList;
		std::vector<struct pollfd> _fds;
		size_t _totalMonitored;

		void createServers( const Parser& configuration );
		bool mergeServerWithSamePort( std::map<std::string, std::string> serverConf );
		void setTotalMonitored( void );
		void setFds( void );
		void handleEvent( void );

		// handleServer()

		// UN-USED CONSTRUCTORS
		Poll( void );
		Poll( const Poll& );
		Poll& operator=( const Poll& );
};

#endif /* __POLL_HPP__ */
