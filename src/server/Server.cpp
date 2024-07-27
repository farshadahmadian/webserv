/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/27 09:24:42 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// new constructor to create the vectors
Server::Server(std::map<std::string, std::string> settings) : _port(settings["port"]),
																															_listeningSocket(ListeningSocket(MAX_CONNECTIONS, settings["server_name"], settings["port"])),
																															_connectedSockets(std::map<int, ConnectedSocket>()),
																															_monitoredFdsNum(0)
																															// _request(std::map<std::string, std::string>())
{ // ----- the problem of the crash seems this! ------ but not here

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

Server::Server(const Server &other) : _port(other._port),
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
	return;
}

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

	// char *ip = new char[100];
	// char ip[100];
	// inet_ntop(this->_addressInfo->ai_family, &((reinterpret_cast<sockaddr_in *>(this->_addressInfo->ai_addr))->sin_addr), ip, 100);
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
	if (fcntl(connectedSocketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl F_SETFL");
	}
	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);

	if (connectedSocket.getSocketFd() == -1)
	{
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}

	this->_connectedSockets[connectedSocketFd] = connectedSocket;

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
				this->handleEventsOnConnectedSockets(i);
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
		// if ((this->_monitoredFds[i].revents & POLLERR) || (this->_monitoredFds[i].revents & POLLHUP) || (this->_monitoredFds[i].revents & POLLNVAL)) {
		// 	throw Exception("Event error", EVENT_ERROR);
		// }

		if ((this->_monitoredFds[i].revents & POLLIN) && (this->_monitoredFdsNum <= MAX_CONNECTIONS))
		{
			int connectedSocketFd = this->acceptFirstRequestInQueue();
			this->addToMonitorsFds(connectedSocketFd);
			this->_monitoredFdsNum++;
		}
		// else {
		// 	throw Exception("Exception in listening socket!", EVENT_ERROR);
		// }
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

void Server::handleEventsOnConnectedSockets(unsigned int i)
{

	std::cout << "this->_monitoredFds[" << i << "].fd = " << this->_monitoredFds[i].fd << std::endl;
	std::cout << "this->_monitoredFds[i].revents = " << std::hex << "0x" << (this->_monitoredFds[i].revents) << std::dec << std::endl;
	std::cout << "this->_monitoredFds[i].revents & POLLIN = " << std::hex << "0x" << (this->_monitoredFds[i].revents & POLLIN) << std::dec << std::endl;
	std::cout << "this->_monitoredFds[i].revents & POLOUT = " << std::hex << "0x" << (this->_monitoredFds[i].revents & POLLOUT) << std::dec << std::endl;
	try
	{
		if (this->_monitoredFds[i].revents & POLLIN)
		{
			std::cout << "RECEIVING THE REQUEST..." << std::endl;
			// naivd_code from here ->
			HTTPRequest httpreq(_settings); // navid_code
			if (httpreq.handleRequest(this->_monitoredFds[i].fd))
			{

				// char receive[20048];
				// receive[20047] = '\0';
				// ssize_t result = recv(this->_monitoredFds[i].fd, receive, sizeof(receive) - 1, 0);
				// if (result == -1)
				// 	throw Exception("Receive Failed", RECEIVE_FAILED);
				// // close(this->_monitoredFds[i].fd);
				// // this->_monitoredFdsNum--;
				// this->parseRequest(static_cast<std::string>(receive));
				// this->printRequest();

				_responses[this->_monitoredFds[i].fd] = httpreq.getResponse(this->_monitoredFds[i].fd);
				std::cout << "Handled request on socket fd " << this->_monitoredFds[i].fd << std::endl;
				this->_monitoredFds[i].events = POLLOUT;
			}
			else
			{
				Exception httpRequestException("httpRequest failed!", HTTP_REQUEST_FAILED);
				throw httpRequestException;
			}
		}

		// if ((this->_monitoredFds[i].revents & POLLOUT) && !(this->_monitoredFds[i].revents & POLLIN))
		// {
		// 	std::cout << "***** NO POLLIN\n";
		// 	// std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
		// 	// 	send(this->_monitoredFds[i].fd, response.c_str(), response.size(), 0);
		// 	// close(this->_monitoredFds[i].fd);
		// 	// this->_monitoredFds[i].fd = -1;
		// 	// this->closeSocket();
		// 	return;
		// }

		if (this->_monitoredFds[i].revents & POLLOUT)
		{

			// std::string htmlContent = this->readHtmlFile("./src/index.html");
			// std::ostringstream ss;
			// ss << "HTTP/1.1 200 OK\r\n";
			// ss << "Content-Type: text/html\r\n";
			// ss << "Content-Length:" << htmlContent.size() << "\r\n";
			// ss << "\r\n";
			// ss << htmlContent;
			// size_t size = ss.str().size();
			// // std::cout << ss.str() << std::endl;
			// send(this->_monitoredFds[i].fd, ss.str().c_str(), size, 0);
			// close(this->_monitoredFds[i].fd);
			// this->_monitoredFdsNum--;
			// ss << response;
			// ss << htmlContent;
			// Send the response

			std::string response = _responses[this->_monitoredFds[i].fd];
			//****************print the provided response in file***********************
			std::cout << RED "****sending the response\n" RESET;
			// writHtmlFile(response, "./src/request/response.txt");
			std::ofstream outfile("./src/request/response.txt");
			outfile << response << std::endl;
			outfile.close();
			//**************************************************************************

			send(this->_monitoredFds[i].fd, response.c_str(), response.size(), 0);
			int closeResult = close(this->_monitoredFds[i].fd);
			if (closeResult == -1)
			{
				std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
			}
			std::cout << RED << "Socket [" << this->_monitoredFds[i].fd << "] is closed." << RESET << std::endl;
			this->_connectedSockets.erase(this->_monitoredFds[i].fd);
			this->_monitoredFds[i].fd = -1;
			this->closeSocket();
			// this->_monitoredFdsNum--;

			// Remove the response from the map
			_responses.erase(this->_monitoredFds[i].fd);
		}
		// ->! to here
		// else
		// {
		// 	throw Exception("Exception in connected socket!", EVENT_ERROR);
		// }
	}
	catch (Exception const &exception)
	{
		throw exception;
	}
	return;
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
	// std::cout << inputStringStream.str() << std::endl;
	std::string method, path, httpVersion;
	inputStringStream >> method;
	inputStringStream >> path;
	inputStringStream >> httpVersion;
	// std::cout << "Method = " << method << std::endl;
	// std::cout << "path = " << path << std::endl;
	// std::cout << "httpVersion = " << httpVersion << std::endl;

	// this->_request["method"] = method;
	// this->_request["path"] = path;
	// this->_request["httpVersion"] = httpVersion;
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

std::map<std::string, std::string> &Server::getSettings(void)
{
	return this->_settings;
}

std::map<int, std::string> &Server::getResponses(void)
{
	return this->_responses;
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

HTTPRequest &Server::getHttpRequest(void) {
	return this->_httpRequest;
}
