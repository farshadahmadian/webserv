/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:39:02 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/05 19:02:39 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HTTPRequest::HTTPRequest( void ) {}

HTTPRequest::~HTTPRequest( void ) { 
	// std::cout << BLUE "HTTPRequest destructor called\n" RESET;
}

HTTPRequest::HTTPRequest(std::map<std::string, std::string> const & serverConfig) :
	_requestString(""),
	_method(""),
	_uri(""),
	_version(""),
	_serverConfig(serverConfig) {
		// std::cout << BLUE "HTTPRequest constructor called\n" RESET;
	}

bool HTTPRequest::isValidMethod(const std::string &mthd)
{
	return (mthd == "GET" || mthd == "POST" || mthd == "HEAD");
}

bool HTTPRequest::isValidHttpVersion(const std::string &ver)
{
	return (ver == "HTTP/1.1" || ver == "HTTP/1.0");
}

bool HTTPRequest::parse()
{
	std::istringstream requestStream(_requestString);
	std::string line;

	// Parse request line
	if (!std::getline(requestStream, line))
	{
		return (false);
	}
	std::istringstream requestLine(line);
	if (!(requestLine >> _method >> _uri >> _version))
	{
		return (false);
	}

	if (!isValidMethod(_method) || !isValidHttpVersion(_version))
	{
		return (false);
	}

	// Parse headers
	_requestMap["version"] = _version;
	_requestMap["uri"] = _uri;
	_requestMap["method"] = _method;

	while (std::getline(requestStream, line) && line != "\r")
	{
		size_t pos = line.find(":");
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
			value.erase(0, value.find_first_not_of(" "));
			_requestMap[key] = value;
		}
	}

	//****************print header map********************
	// displayRequestMap();
	//****************print srver config map**************
	// displayServerConfig();
	//****************************************************

	return true;
}

std::string const & HTTPRequest::getRequestString() const {
	return (_requestString);
}

std::map<std::string, std::string> & HTTPRequest::getRequestMap() {return (_requestMap);}

void HTTPRequest::displayRequestString() const
{
	std::cout << RED "****Request String:\n"
		<< CYAN << _requestString << RESET << std::endl;
}

std::map<std::string, std::string> &HTTPRequest::getServerConfig(void) {
	return this->_serverConfig;
}

void HTTPRequest::displayRequestMap()
{
	std::cout << RED "****Request Map:\n";
	std::map<std::string, std::string>::iterator itr = _requestMap.begin();
	for (; itr != _requestMap.end(); itr++)
		std::cout << ORG << itr->first << ":" MAGENTA << itr->second << RESET << std::endl;
}

void HTTPRequest::displayServerConfig()
{
	std::cout << RED "****The server config map:\n";
	std::map<std::string, std::string>::iterator itrr = _serverConfig.begin();
	for (; itrr != _serverConfig.end(); itrr++)
		std::cout << ORG << itrr->first << "->" MAGENTA << itrr->second << RESET << std::endl;
}

bool HTTPRequest::handleRequest(int clientSocket)
{
	char buffer[40960];
	ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	// std::cout << CYAN << "bytesRead = " << bytesRead << RESET << std::endl;
	if (bytesRead == -1) {// && (errno == EAGAIN || errno == EWOULDBLOCK)){
		return (false);
	}

	if (bytesRead < 0)
	{
		throw Exception("Receive on clientSocket Failed", CLIENTSOCKET_RECEIVE_FAILED);
		close(clientSocket);
		return (false);
	}

	buffer[bytesRead] = '\0';
	_requestString.assign(buffer);
	// std::cout << _requestString << std::endl;

	//****************print request***********************
	// displayRequestString();
	//****************************************************

	if (!parse())
	{
		std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
		send(clientSocket, response.c_str(), response.length(), 0);
		close(clientSocket);
		return (false);
	}
	return (true);
}

void writStringtoFile(std::string request, std::string path)
{
	std::istringstream streamTest(request);
	std::string stringTest;
	std::ofstream outFile(path.c_str());
	while (std::getline(streamTest, stringTest))
	{
		std::cout << ORG << stringTest << RESET << std::endl;
		// Manually escape special characters
		if (outFile.is_open())
		{
			for (std::string::size_type i = 0; i < stringTest.size(); ++i)
			{
				char c = stringTest[i];
				switch (c)
				{
				case '\r': outFile << "'\\r'"; break;
				case '\n': outFile << "'\\n'"; break;
				case '\t': outFile << "'\\t'"; break;
				case '\v': outFile << "'\\v'"; break;
				case '\f': outFile << "'\\f'"; break;
				case ' ' : outFile << "' '"  ; break;
				default: outFile << c;
				}
			}
		}
	}
	outFile.close();
	exit(1);
}
