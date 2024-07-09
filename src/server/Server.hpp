/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 16:43:31 by nnabaeei         ###   ########.fr       */
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

class Server {
	private:
		ListeningSocket _listeningSocket;
		// std::vector<ConnectedSocket> _connectedSockets;
		std::map<int, ConnectedSocket> _connectedSockets;
		unsigned int _maxIncomingConnections;
		unsigned int _monitoredFdsNum;
		struct pollfd *_monitoredFds;
		std::map<std::string, std::string> _request;

		void handleEvents(void);
		void handleEventsOnListeningSocket(unsigned int i);
		void handleEventsOnConnectedSockets(unsigned int i);
		void addToMonitorsFds(int connectedSocketFd);
		std::string readHtmlFile(std::string path);
		void parseRequest(std::string request);
		void printRequest(void);
	public:
		Server( std::map<std::string, std::string> settings ); // new from nico
		Server(void);
		Server(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port);
		Server(Server const &other);
		Server &operator=(Server const &rhs);
		~Server(void);

		ListeningSocket const &getListeningSocket(void) const;
		
		void printConnectedSockets(void);

		void setPortAvailable(void);
		void bindSocket(void) const;
		void listenToRequests(void) const;
		int acceptFirstRequestInQueue(void);
		void startPoll(void);
		void startPoll2(void);
		
};


#endif /* __SERVER_HPP__ */
