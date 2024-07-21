/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/21 09:41:40 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string>
#include <netdb.h>
#include <fcntl.h>
#include <stdexcept>
#include <vector>
#include <arpa/inet.h>
#include <poll.h>
#include <map>
#include <fstream>
#include "../request/HttpRequest.hpp"
#include <sstream>
#include "Exception.hpp"
#include "../sockets/listening-socket/ListeningSocket.hpp"
#include "../sockets/connected-socket/ConnectedSocket.hpp"
#include "../server/Server.hpp"

#define MAX_CONNECTIONS 10

class Server
{
private:
	const std::string _serverName;
	const std::string _root;
	ListeningSocket _listeningSocket;
	// std::vector<ConnectedSocket> _connectedSockets;
	std::map<int, ConnectedSocket> _connectedSockets;
	unsigned int _monitoredFdsNum;
	struct pollfd _monitoredFds[MAX_CONNECTIONS];
	std::map<std::string, std::string> _request;
	std::map<int, std::string> _responses;				// navid_code
	std::map<std::string, std::string> _settings; // navid_code

	void handleEvents(void);
	void handleEventsOnListeningSocket(unsigned int i);
	void handleEventsOnConnectedSockets(unsigned int i);
	void addToMonitorsFds(int connectedSocketFd);
	std::string readHtmlFile(std::string path);
	void parseRequest(std::string request);
	void printRequest(void);
	void closeSocket(void);

	Server(void); // ---------------------------------- UNUSED CONSTRUCTORS
	Server(Server const &other);
	Server &operator=(Server const &rhs);

public:
	Server(std::map<std::string, std::string> settings);
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
