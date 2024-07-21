/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/21 09:31:38 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <string>
# include <netdb.h>
# include <stdexcept>
# include <vector>
# include <arpa/inet.h>
# include <poll.h>
# include <map>
# include <fstream>
# include <sstream>
# include "./exception/Exception.hpp"
# include "../sockets/listening-socket/ListeningSocket.hpp"
# include "../sockets/connected-socket/ConnectedSocket.hpp"
# include "../server/Server.hpp"

#define MAX_CONNECTIONS		10

class Server {
	private:
		// new elements
		const std::string _port;
		std::vector<std::string> _serverNames;
		std::vector<std::string> _roots;
		std::vector<std::string> _indexes;
		// std::vector<std::string> _keepaliveTimeouts;
		// std::vector<std::string> _autoindexes;
		// std::vector<std::string> _clientSizes;

		ListeningSocket _listeningSocket;
		// std::vector<ConnectedSocket> _connectedSockets;
		std::map<int, ConnectedSocket> _connectedSockets;
		unsigned int _monitoredFdsNum;
		struct pollfd _monitoredFds[MAX_CONNECTIONS];
		std::map<std::string, std::string> _request;


		void handleEvents(void);
		void handleEventsOnListeningSocket(unsigned int i);
		void handleEventsOnConnectedSockets(unsigned int i);
		void addToMonitorsFds(int connectedSocketFd);
		std::string readHtmlFile(std::string path);
		void parseRequest(std::string request);
		void printRequest(void);

		Server(	void ); // ---------------------------------- UNUSED CONSTRUCTORS
		Server& operator=( Server const &rhs );
	public:
		Server( std::map<std::string, std::string> settings );
		~Server( void );
		Server( const Server& other );

		ListeningSocket const &getListeningSocket(void) const;

		void printConnectedSockets(void);

		const std::string getPort( void ) const;
		size_t getMonitoredFdsNum( void ) const;

		void addServerName( std::string newName );
		void addRoot( std::string newRoot );
		void addIndex( std::string newIndex );

		void setPortAvailable(void);
		void bindSocket(void) const;
		void listenToRequests(void) const;
		int acceptFirstRequestInQueue(void);
		void startPoll(void);
		void startPoll2(void);
};

#endif /* __SERVER_HPP__ */
