/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/08 13:06:25 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <string>
# include <netdb.h>
# include <stdexcept>
# include <vector>
# include <arpa/inet.h>
# include <poll.h>
# include <map>
# include <fstream>
# include <sstream>
# include "Exception.hpp"
# include "ListeningSocket.hpp"
# include "ConnectedSocket.hpp"

class HttpServer {
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
		HttpServer(void);
		HttpServer(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port);
		HttpServer(HttpServer const &other);
		HttpServer &operator=(HttpServer const &rhs);
		~HttpServer(void);

		ListeningSocket const &getListeningSocket(void) const;
		
		void printConnectedSockets(void);

		void setPortAvailable(void);
		void bindSocket(void) const;
		void listenToRequests(void) const;
		int acceptFirstRequestInQueue(void);
		void startPoll(void);
		void startPoll2(void);
		
};


#endif
