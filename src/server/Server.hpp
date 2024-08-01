/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/01 23:10:17 by nnabaeei         ###   ########.fr       */
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
#include "../request/HttpResponse.hpp"
#include <sstream>
#include "Exception.hpp"
#include "../sockets/listening-socket/ListeningSocket.hpp"
#include "../sockets/connected-socket/ConnectedSocket.hpp"
#include "../server/Server.hpp"
#include "HttpRequest.hpp"

#define MAX_CONNECTIONS 3

class Server
{
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
	std::map<int, ConnectedSocket> _connectedSockets;
	unsigned int _monitoredFdsNum;
	struct pollfd _monitoredFds[MAX_CONNECTIONS]; // obsolete
	std::map<std::string, std::string> _request;

	// std::map<int, std::string> _responses;				// navid_code
	// std::map<std::string, std::string> _settings; // navid_code
	HTTPRequest _httpReq; //navid_code
	HTTPResponse _httpResp; //navid_code

	void handleEvents(void);
	void handleEventsOnListeningSocket(unsigned int i);
	void handleEventsOnConnectedSockets(unsigned int i);
	void addToMonitorsFds(int connectedSocketFd);
	std::string readHtmlFile(std::string path);
	void parseRequest(std::string request);
	void printRequest(void);

	Server(void); // ---------------------------------- UNUSED CONSTRUCTORS
	Server &operator=(Server const &rhs);

public:
	Server(std::map<std::string, std::string> settings);
	~Server(void);
	Server(const Server &other);

	ListeningSocket const &getListeningSocket(void) const;

	void printConnectedSockets(void);

	const std::string getPort(void) const;
	size_t getMonitoredFdsNum(void) const;
	std::map<int, ConnectedSocket> &getConnectedSockets(void);
	// std::map<std::string, std::string> &getServerConf(void);//navid_code
	// std::map<int, std::string> &getResponses(void);

	void addServerName(std::string newName);
	void addRoot(std::string newRoot);
	void addIndex(std::string newIndex);

	void setPortAvailable(void);
	void bindSocket(void) const;
	void listenToRequests(void) const;
	int acceptFirstRequestInQueue(void);
	void closeSocket(void);

	HTTPRequest & getHttpReq(); //navid_code
	HTTPResponse & getHttpResp(); //navid_code
};

#endif /* __SERVER_HPP__ */
