/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/10 11:33:46 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// new constructor to create the vectors
Server::Server(std::map<std::string, std::string> settings) :
	_port(settings["port"]),
	_listeningSocket(ListeningSocket(MAX_CONNECTIONS, settings["server_name"], settings["port"])),
	_connectedSockets(std::map<int, ConnectedSocket>()),
	_httpReq(settings),
	_httpResp(settings)
{
	_serverNames.push_back(settings["server_name"]);
	_roots.push_back(settings["root"]);
	_indexes.push_back(settings["index"]);
	_keepAliveTimeout = stringToInt(settings["keepalive_timeout"]);
	// _keepaliveTimeouts.push_back(settings["keepalive_timeout"]);
	// _autoindexes.push_back(settings["autoindex"]);
	// _clientSizes.push_back(settings["client_body_buffer_size"]);

	// std::cout << GREEN << "* Server [ " << settings["server_name"] << " ] created successfully." << RESET << std::endl;
}

Server::Server(const Server &other) :
	_port(other._port),
	_serverNames(other._serverNames),
	_roots(other._roots),
	_indexes(other._indexes),
	_keepAliveTimeout(other._keepAliveTimeout),
	// _autoindexes(other._autoindexes),
	// _clientSizes(other._clientSizes),
	_listeningSocket(other._listeningSocket),
	_connectedSockets(other._connectedSockets),
	_httpReq(other._httpReq),
	_httpResp(other._httpResp)
{
	return;
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

Server &Server::operator=(Server const &rhs) {
	(void)rhs;
	return *this;
}

HTTPRequest& Server::getHttpReq() {
    return (_httpReq);
}

HTTPResponse& Server::getHttpResp() {
	_httpResp.setRequestMapInResponse(_httpReq.getRequestMap());
	_httpResp.setRequestStringInResponse(_httpReq.getRequestString());
    return (_httpResp);
}

ListeningSocket const &Server::getListeningSocket(void) const { return (this->_listeningSocket); }

void Server::printConnectedSockets(void)
{
	if (!(this->_connectedSockets.size()))
	{
		std::cout << "connectedSockets map in " << this->_listeningSocket.getSocketFd() << " is empty" << std::endl;
		return;
	}
	std::map<int, ConnectedSocket>::iterator iterator;
	std::map<int, ConnectedSocket>::iterator iteratorEnd = this->_connectedSockets.end();

	std::cout << "Connected Sockets List in Server: " << this->_listeningSocket.getSocketFd() << " ===> " << std::endl;
	for (iterator = this->_connectedSockets.begin(); iterator != iteratorEnd; iterator++)
		std::cout << "connectedSocket.key = " << iterator->first << " connectedSocket.value = " << iterator->second.getSocketFd() << "isConencted = " << iterator->second.getIsConnected() << std::endl;

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
	// std::cout << GREEN << "Listening Socket with fd(" << this->_listeningSocket.getSocketFd() << ") of type " << (this->_listeningSocket.getAddressInfo()->ai_socktype == SOCK_STREAM ? "TCP Socket" : "UNKNOWN SOCKET!!!!") << " created" << RESET << std::endl;
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
	// std::cout << GREEN << "Listening Socket is bound to " << ip.c_str() << ":" << ntohs(reinterpret_cast<sockaddr_in *>(this->_listeningSocket.getAddressInfo()->ai_addr)->sin_port) << RESET << std::endl;

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
	if (connectedSocketFd < 0)
	{
		Exception exception("Accepting the request failed", ACCEPTING_FAILED);
		throw exception;
	}
	ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);
	this->_connectedSockets[connectedSocketFd] = connectedSocket;

	this->_connectedSockets[connectedSocketFd].setConnectionStartTime();
	this->_connectedSockets[connectedSocketFd].setState(this->_keepAliveTimeout ? KEEP_ALIVE : CLOSED);

	// std::cout << YELLOW << "***********Incoming Connection Address***********:" << RESET << std::endl;
	// std::string clientIp =  inet_ntoa(reinterpret_cast<sockaddr_in *>(&incomingConnectionAddress)->sin_addr); //remove
	// std::cout << "Client address is " << clientIp << ":" << ntohs(reinterpret_cast<sockaddr_in *>(&incomingConnectionAddress)->sin_port) << std::endl; //remove
	// std::cout << YELLOW << "*************************************************:" << RESET << std::endl;
	std::cout << GREEN << "Connected socket with fd(" << connectedSocket.getSocketFd() << ") is created" << RESET << std::endl;

	return (connectedSocketFd);
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

<<<<<<< HEAD
const std::string Server::getPort(void) const {
=======
const std::string Server::getPort(void) const {
>>>>>>> multiple_servers_tmp05
	return (_port);
};

std::map<int, ConnectedSocket> &Server::getConnectedSockets(void)
{
	return this->_connectedSockets;
}

int Server::getKeepAliveTimeout(void) const {
	return this->_keepAliveTimeout;
}

void Server::setKeepAliveTimeout(int keepAliveTimeout) {
	this->_keepAliveTimeout = keepAliveTimeout;
	return;
}

void Server::addServerName(std::string newName) { _serverNames.push_back(newName); };
void Server::addRoot(std::string newRoot) { _roots.push_back(newRoot); };
void Server::addIndex(std::string newIndex) { _indexes.push_back(newIndex); };

int Server::stringToInt(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (ss.fail()) {
        throw std::runtime_error("Error: Invalid number");
    }
    return value;
}
