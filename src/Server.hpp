/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 15:59:39 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <iostream>
#include <vector>
#include <map>
#include "Location.hpp"
#include "colors.h"

// farshad
#include "ListeningSocket.hpp"

class Server
{
	public:
		Server( size_t ); // -------------------------------- USED CONSTRUCTORS
		~Server( void );
		Server( const Server& );
		
		size_t _i; // ------------------------------------------------ SETTINGS
		size_t _n_location;
		std::vector<Location> _location;
		std::map<std::string, std::string> _settings;
		size_t getIndex( void );
		std::vector<Location>& getLocation( void );
		std::map<std::string, std::string> getSettings( void ); /////// ?????? need to return reference ???????????
		void setSettings( std::string key, std::string value );
		void addLocation( void );

		void displayServerSettings( void ); // ------------------------ DISPLAY
		
		void start( void ); // ------------------------------------------- CORE
	
	private:
		ListeningSocket _listeningSocket;					// need to initialize all the stuff ?
		std::map<int, ConnectedSocket> _connectedSockets;
		unsigned int _maxIncomingConnections;
		unsigned int _monitoredFdsNum;
		struct pollfd *_monitoredFds;
		std::map<std::string, std::string> _request;

	// 	void handleEvents(void);
	// 	void handleEventsOnListeningSocket(unsigned int i);
	// 	void handleEventsOnConnectedSockets(unsigned int i);
	// 	void addToMonitorsFds(int connectedSocketFd);
	// 	std::string readHtmlFile(std::string path);
	// 	void parseRequest(std::string request);
	// 	void printRequest(void);
	// public:
	// 	HttpServer(void);
	// 	HttpServer(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port);
	// 	HttpServer(HttpServer const &other);
	// 	HttpServer &operator=(HttpServer const &rhs);
	// 	~HttpServer(void);

	// 	ListeningSocket const &getListeningSocket(void) const;
		
	// 	void printConnectedSockets(void);

	// 	void setPortAvailable(void);
	// 	void bindSocket(void) const;
	// 	void listenToRequests(void) const;
	// 	int acceptFirstRequestInQueue(void);
	// 	void startPoll(void);
	// 	void startPoll2(void);
		// ----------------------------------------------------------- END CORE
		
	
	private:
		Server( void ); // -------------------------------- UNUSED CONSTRUCTORS
		void operator=( const Server& );
};

#endif /* __SERVER_HPP__ */
