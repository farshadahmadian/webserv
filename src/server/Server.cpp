/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/01 08:37:45 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// new constructor to create the vectors
Server::Server(std::map<std::string, std::string> settings) : 
	_port(settings["port"]),
	_listeningSocket(ListeningSocket(MAX_CONNECTIONS, settings["server_name"], settings["port"])),
	_connectedSockets(std::map<int, ConnectedSocket>()),
	_monitoredFdsNum(0),
	_httpReq(settings),
	_httpResp(settings)
	// _request(std::map<std::string, std::string>())
{ // ----- the problem of the crash seems this! ------ but not here
	std::cout << BLUE << "Server settings map constructor called" RESET << std::endl;
	_serverNames.push_back(settings["server_name"]);
	_roots.push_back(settings["root"]);
	_indexes.push_back(settings["index"]);
	// _keepaliveTimeouts.push_back(settings["keepalive_timeout"]);
	// _autoindexes.push_back(settings["autoindex"]);
	// _clientSizes.push_back(settings["client_body_buffer_size"]);

	// //// this->_monitoredFds = new struct pollfd[MAX_CONNECTIONS + 1];
	memset(this->_monitoredFds, 0, sizeof(struct pollfd) * (MAX_CONNECTIONS + 1));
	for (unsigned int i = 0; i < MAX_CONNECTIONS + 1; i++)
		this->_monitoredFds[i].fd = -1;
	this->_monitoredFds[0].fd = this->_listeningSocket.getSocketFd();
	this->_monitoredFds[0].events = POLLIN;
	this->_monitoredFdsNum++;

	std::cout << GREEN << "* Server [ " << settings["server_name"] << " ] created successfully." << RESET << std::endl;
}

Server::Server(const Server &other) : 
	_port(other._port),
	_serverNames(other._serverNames),
	_roots(other._roots),
	_indexes(other._indexes),
	// _keepaliveTimeouts(other._keepaliveTimeouts),
	// _autoindexes(other._autoindexes),
	// _clientSizes(other._clientSizes),
	_listeningSocket(other._listeningSocket),
	_connectedSockets(other._connectedSockets),
	_monitoredFdsNum(other._monitoredFdsNum)
	// _request(other._request) // ----- the problem of the crash seems this! ------
{
	std::cout << RED << "Server copy constructor called" RESET << std::endl;

	size_t i = 0;
	while (i < _monitoredFdsNum)
	{
		_monitoredFds[i] = other._monitoredFds[i];
		i++;
	}
}

Server::~Server(void)
{
	// close(this->_listeningSocket.getSocketFd());

	// std::map<int, ConnectedSocket>::iterator iterator;
	// std::map<int, ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	// for (iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
	// 	close(iterator->second.getSocketFd());

	// delete this->_monitoredFds;
	std::cout << BLUE << "Server destructor called" RESET << std::endl;
	return;
}
//navid_code from here -->
HTTPRequest& Server::getHttpReq() {
    return (_httpReq);
}

HTTPResponse& Server::getHttpResp() {
	_httpResp.setResponseRequestMap(_httpReq.getRequestMap());
	_httpResp.setResponseRequestString(_httpReq.getRequestString());
    return (_httpResp);
}
//navid_code to here <---

ListeningSocket const &Server::getListeningSocket(void) const { return (this->_listeningSocket); }

void Server::printConnectedSockets(void)
{
	std::map<int, ConnectedSocket>::iterator iterator;
	std::map<int, ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	std::cout << "Connected Sockets List in: " << this->_listeningSocket.getSocketFd() << " ===> " << std::endl;
	for (iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
		std::cout << "connectedSocket.key = " << iterator->first << " connectedSocket.value = " << iterator->second.getSocketFd() << std::endl;

	std::cout << "finish\n";
}

void Server::setPortAvailable(void)
{
	int reusePort = 1;
	int setSocketOptionResult = setsockopt(this->_listeningSocket.getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	if (setSocketOptionResult == -1)
	{
		Exception exception("Setting socket options faild!", SOCKET_OPTIONS_FAILD);
		throw exception;
	}
	std::cout << GREEN << "Listening Socket with fd(" << this->_listeningSocket.getSocketFd() << ") of type " << (this->_listeningSocket.getAddressInfo()->ai_socktype == SOCK_STREAM ? "TCP Socket" : "UNKNOWN SOCKET!!!!") << " created" << RESET << std::endl;
	return;
}

void Server::bindSocket(void) const
{
	int bindResult = bind(this->_listeningSocket.getSocketFd(), this->_listeningSocket.getAddressInfo()->ai_addr, this->_listeningSocket.getAddressInfo()->ai_addrlen);
	if (bindResult != 0)
	{
		Exception exception("Binding the socket to the address failed!", BIND_SOCKET_FAILD);
		throw exception;
	}
	std::string ip = static_cast<std::string>(inet_ntoa(reinterpret_cast<sockaddr_in *>(this->_listeningSocket.getAddressInfo()->ai_addr)->sin_addr)); // REMOVE
	std::cout << GREEN << "Listening Socket is bound to " << ip.c_str() << ":" << ntohs(reinterpret_cast<sockaddr_in *>(this->_listeningSocket.getAddressInfo()->ai_addr)->sin_port) << RESET << std::endl;

	return;
}

void Server::listenToRequests(void) const
{
	int listenResult = listen(this->_listeningSocket.getSocketFd(), static_cast<int>(this->_listeningSocket.getMaxIncomingConnections()));
	if (listenResult == -1)
	{
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}

	std::cout << GREEN << "Listening socket is litening to requests" << RESET << std::endl;
	return;
}

int Server::acceptFirstRequestInQueue(void)
{

	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	int connectedSocketFd = accept(this->_listeningSocket.getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);
	// if (fcntl(connectedSocketFd, F_SETFL, O_NONBLOCK) == -1)
	// {
	// 	perror("fcntl F_SETFL");
	// }
	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);

	if (connectedSocket.getSocketFd() == -1)
	{
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}

	this->_connectedSockets[connectedSocketFd] = connectedSocket;

	std::cout << YELLOW << "***********Incoming Connection Address***********:" << RESET << std::endl;
	std::string clientIp =  inet_ntoa(reinterpret_cast<sockaddr_in *>(&incomingConnectionAddress)->sin_addr); //remove
	std::cout << "Client address is " << clientIp << ":" << ntohs(reinterpret_cast<sockaddr_in *>(&incomingConnectionAddress)->sin_port) << std::endl; //remove
	std::cout << YELLOW << "*************************************************:" << RESET << std::endl;
	std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") is created" << RESET << std::endl;

	return (connectedSocketFd);
}

void Server::handleEvents(void)
{
	try
	{
		for (unsigned int i = 0; i < this->_monitoredFdsNum; i++)
		{
			// navid_code from here ->
			if (this->_monitoredFds[i].revents == 0)
			{
				continue;
			}
			if (this->_monitoredFds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				close(this->_monitoredFds[i].fd);
				this->_connectedSockets.erase(this->_monitoredFds[i].fd);
				this->_monitoredFds[i].fd = -1;
				this->closeSocket();
				continue;
			}
			// ->! to here
			if (this->_monitoredFds[i].fd == this->_listeningSocket.getSocketFd())
			{
				std::cout << "handling event on the listening socket" << std::endl;
				this->handleEventsOnListeningSocket(i);
			}
			else
			{
				std::cout << "handling event on a connected socket" << std::endl;
				// this->handleEventsOnConnectedSockets(i);
			}
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
}

void Server::handleEventsOnListeningSocket(unsigned int i)
{
	try
	{
		if ((this->_monitoredFds[i].revents & POLLIN) && (this->_monitoredFdsNum <= MAX_CONNECTIONS))
		{
			int connectedSocketFd = this->acceptFirstRequestInQueue();
			this->addToMonitorsFds(connectedSocketFd);
			this->_monitoredFdsNum++;
		}
	}
	catch (Exception const &exception)
	{
		throw exception;
	}

	return;
}

void Server::addToMonitorsFds(int connectedSocketFd)
{
	for (unsigned int i = 0; i <= MAX_CONNECTIONS; i++)
	{
		if (this->_monitoredFds[i].fd == -1)
		{
			this->_monitoredFds[i].fd = connectedSocketFd;
			// this->_monitoredFds[i].events = POLLIN | POLLOUT;
			this->_monitoredFds[i].revents = 0;
			this->_monitoredFds[i].events = POLLIN;
			return;
		}
	}
}

std::string Server::readHtmlFile(std::string path)
{
	std::ifstream fileStream(path.c_str());
	// fileStream.open(path.c_str());
	if (fileStream.is_open())
		std::cout << "file is open\n";
	else
	{
		perror("error:");
		return ("");
	}
	std::ostringstream ss;
	ss << fileStream.rdbuf();
	// std::cout << ss.str() << std::endl;
	return ss.str();
}

void Server::parseRequest(std::string request)
{
	std::istringstream inputStringStream(request, std::ios::in);
	std::cout << "Parsing the request:" << std::endl;
	std::string method, path, httpVersion;
	inputStringStream >> method;
	inputStringStream >> path;
	inputStringStream >> httpVersion;
}

void Server::printRequest(void)
{
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_request.end();
	std::cout << "PRINTING CONNECTED SOCKET LIST ON " << this->_listeningSocket.getSocketFd() << std::endl;
	for (iterator = this->_request.begin(); iterator != iteratorEnd; iterator++)
		std::cout << iterator->first << " = " << iterator->second << std::endl;
	return;
}

const std::string Server::getPort(void) const { 
	return (_port);
};

size_t Server::getMonitoredFdsNum(void) const { 
	return (_monitoredFdsNum);
};

std::map<int, ConnectedSocket> &Server::getConnectedSockets(void)
{
	return this->_connectedSockets;
}

void Server::addServerName(std::string newName) { _serverNames.push_back(newName); };
void Server::addRoot(std::string newRoot) { _roots.push_back(newRoot); };
void Server::addIndex(std::string newIndex) { _indexes.push_back(newIndex); };
void Server::closeSocket()
{
	for (unsigned int i = 0; i < this->_monitoredFdsNum;)
	{
		if (this->_monitoredFds[i].fd == -1)
		{
			for (unsigned int j = i; j < this->_monitoredFdsNum - 1; ++j)
			{
				this->_monitoredFds[j] = this->_monitoredFds[j + 1];
				this->_monitoredFds[j + 1].fd = -1;
			}
			this->_monitoredFdsNum--;
		}
		else
		{
			i++;
		}
	}
}
