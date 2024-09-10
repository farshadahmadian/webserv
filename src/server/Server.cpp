/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/10 11:00:58 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::map<std::string, std::string> settings, std::vector<LocationConf> const &locations) :
	_port(settings["port"]),
	_listeningSocket(ListeningSocket(MAX_CONNECTIONS, settings["server_name"], settings["port"])),
	_connectedSockets(std::map<int, ConnectedSocket>()),
	_httpReq(settings),
	_httpResp(settings, locations)
{
	_serverNames.push_back(settings["server_name"]);
	_roots.push_back(settings["root"]);
	_indexes.push_back(settings["index"]);
	_keepAliveTimeout = stringToInt(settings["keepalive_timeout"]);
	if (_keepAliveTimeout == 0)
		logMessage("Warning: KeepAlive Is Wrong Or Zero!, And Default Is Set.");
}

Server::Server(const Server &other) :
	_port(other._port),
	_serverNames(other._serverNames),
	_roots(other._roots),
	_indexes(other._indexes),
	_keepAliveTimeout(other._keepAliveTimeout),
	_listeningSocket(other._listeningSocket),
	_connectedSockets(other._connectedSockets),
	_httpReq(other._httpReq),
	_httpResp(other._httpResp)
{
	return;
}

Server::~Server(void)
{
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
		logMessage("ERROR: Setting socket options failed!");
		Exception exception("Setting socket options failed!", SOCKET_OPTIONS_FAILED);
		throw exception;
	}
	return;
}

void Server::bindSocket(void) const
{
	int bindResult = bind(this->_listeningSocket.getSocketFd(), this->_listeningSocket.getAddressInfo()->ai_addr, this->_listeningSocket.getAddressInfo()->ai_addrlen);
	if (bindResult != 0)
	{
		logMessage("ERROR: Binding the socket to the address failed!");
		Exception exception("Binding the socket to the address failed!", BIND_SOCKET_FAILED);
		throw exception;
	}

	return;
}

void Server::listenToRequests(void) const
{
	int listenResult = listen(this->_listeningSocket.getSocketFd(), static_cast<int>(this->_listeningSocket.getMaxIncomingConnections()));
	if (listenResult == -1)
	{
		logMessage("ERROR: Listening to incoming connections failed!");
		Exception exception("Listening to incoming connections failed!", LISTENING_FAILED);
		throw exception;
	}
	return;
}

int Server::acceptFirstRequestInQueue(bool addToConnectedSocketsList)
{

	struct sockaddr_storage incomingConnectionAddress;
	memset(&incomingConnectionAddress, 0, sizeof(incomingConnectionAddress));
	socklen_t incomingConnectionAddressSize = static_cast<socklen_t>(sizeof(incomingConnectionAddress));

	int connectedSocketFd = accept(this->_listeningSocket.getSocketFd(), reinterpret_cast<sockaddr *>(&incomingConnectionAddress), &incomingConnectionAddressSize);

	if (fcntl(connectedSocketFd, F_SETFL, O_NONBLOCK) == -1)
		Server::logMessage("ERROR: The fcntl F_SETFL Error Set!");
	if (connectedSocketFd < 0)
	{
		Server::logMessage("ERROR: Accepting the request failed");
		return (ACCEPTING_FAILED);
	}

	if (addToConnectedSocketsList) {
		ConnectedSocket connectedSocket(connectedSocketFd, incomingConnectionAddress, incomingConnectionAddressSize);
		this->_connectedSockets[connectedSocketFd] = connectedSocket;
		this->_connectedSockets[connectedSocketFd].setConnectionStartTime();
		this->_connectedSockets[connectedSocketFd].setState(this->_keepAliveTimeout ? KEEP_ALIVE : CLOSED);
	}
	return (connectedSocketFd);
}

std::string Server::readHtmlFile(std::string path)
{
	std::ifstream fileStream(path.c_str());
	if (fileStream.is_open())
		std::cout << "file is open\n";
	else
	{
		perror("error:");
		return ("");
	}
	std::ostringstream ss;
	ss << fileStream.rdbuf();
	fileStream.close();
	return ss.str();
}

const std::string Server::getPort(void) const {
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


std::string Server::intToString(int const i) {
	std::ostringstream convert;
	convert << i;
	return (convert.str());
}

int Server::stringToInt(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (ss.fail())
			return 0;
    return value;
}


void Server::logMessage(const std::string &message) {
    const std::string logFileName = "./src/server.log";

    struct stat buffer;
    if (stat(logFileName.c_str(), &buffer) != 0) {
        std::ofstream createLogFile(logFileName.c_str());
        if (!createLogFile.is_open()) {
            std::cerr << "Error: Unable to create log file!" << std::endl;
            return;
        }
    }

    std::ofstream logFile(logFileName.c_str(), std::ios_base::app);
    if (logFile.is_open()) {
			std::time_t now = std::time(NULL);
			char timeBuffer[80];
			std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
			logFile << "[" << timeBuffer << "] " << message << std::endl;
    } else {
        std::cerr << "Error: Unable to open log file!" << std::endl;
    }
}
