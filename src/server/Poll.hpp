/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 10:54:45 by ncasteln          #+#    #+#             */
/*   Updated: 2024/09/03 18:37:03 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __POLL_HPP__
#define __POLL_HPP__

#include "Server.hpp"
#include "Parser.hpp"
#include <iostream>

class Poll
{
public:
	Poll(const Parser &configuration);
	~Poll(void);

	void init(void);
	void start(void);
	nfds_t mapConnectedSocketFdToPollFd(int connectedSocketFd);
	void cleanConnectedSockets(void);

	int static cgiChildProcessNum;

private:
	std::vector<Server> _serverList;
	nfds_t _currentMonitored;
	size_t _totalMonitored;
	struct pollfd *_totalFds;

	std::map<int, int> _POLLINCheck; //navid_check

	void createServers(const Parser &configuration);
	bool mergeServerWithSamePort(std::map<std::string, std::string> serverConf);
	void initFds(void);

	void handleEvent(void);
	void handleListeningEvent(size_t i, Server &s);
	void handleConnectedEvent(int i, Server &s, std::map<int, ConnectedSocket>::iterator *connectedSocketIt);
	void addConnectedSocketToMonitoredList(int connectedSocketFd);
	void removeClosedSocketsFromMap(Server &s);
	void removeClosedSocketsFromPollFds(void);
	void printCurrentPollFds(void);
	void printAllPollFds(void);
	bool isMaxConnection(Server &s, size_t i);
	bool receiveRequest(Server &s, size_t i, int connectedSocketFd, std::map<int, ConnectedSocket>::iterator *connectedSocketIt);
	void sendResponse(Server &s, size_t i, int connectedSocketFd, std::map<int, ConnectedSocket>::iterator *connectedSocketIt);
	void closeTimedoutSockets(nfds_t pollNum, ConnectedSocket &connectedSocket);
	std::string waitForCgiResponse(ConnectedSocket &connectedSocket, Server &s);
	void finishCgi(ConnectedSocket &connectedSocket, Server &s, std::string const &response);
	std::string cgiChildProcessSuccess(ConnectedSocket &connectedSocket, Server &s);
	std::string cgiChildProcessFail(ConnectedSocket &connectedSocket, Server &s);

	// UN-USED CONSTRUCTORS
	Poll(void);
	Poll(const Poll &);
	Poll &operator=(const Poll &);
};

#endif /* __POLL_HPP__ */
