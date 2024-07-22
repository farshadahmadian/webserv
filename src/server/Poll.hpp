/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:54:45 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/22 10:00:55 by ncasteln         ###   ########.fr       */
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
		size_t _currentMonitored;
		size_t _totalMonitored;
		struct pollfd* _totalFds;

		void createServers( const Parser& configuration );
		bool mergeServerWithSamePort( std::map<std::string, std::string> serverConf );
		void initFds( void );

		void handleEvent( void );
		void handleListeningEvent( size_t i, Server& s );
		void handleConnectedEvent( size_t i, Server& s );
		void updateFds( int connectedSocketFd );

		// UN-USED CONSTRUCTORS
		Poll( void );
		Poll( const Poll& );
		Poll& operator=( const Poll& );
};

#endif /* __POLL_HPP__ */
