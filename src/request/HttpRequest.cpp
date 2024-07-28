/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:39:02 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/28 11:10:30 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HTTPRequest::HTTPRequest( void ) {}

HTTPRequest::~HTTPRequest( void ) { std::cout << BLUE "HTTPRequest destructor called\n" RESET;}

HTTPRequest::HTTPRequest(std::map<std::string, std::string> &serverConfig) :
	_requestString(""),
	_method(""),
	_uri(""),
	_version(""),
	_body(""),
	_serverConfig(serverConfig) {std::cout << BLUE "HTTPRequest constructor called\n" RESET;}

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
		return false;
	}
	std::istringstream requestLine(line);
	if (!(requestLine >> _method >> _uri >> _version))
	{
		return false;
	}

	if (!isValidMethod(_method) || !isValidHttpVersion(_version))
	{
		return false;
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
	displayHeaders();
	//****************print srver config map**************
	displayServerConfig();
	//****************************************************

	return true;
}

std::map<std::string, std::string> const &HTTPRequest::getRequest() {return (_requestMap);}

void HTTPRequest::displayRequest() const
{
	std::cout << RED "****received request:\n"
		<< CYAN << _requestString << RESET << std::endl;
}

void HTTPRequest::displayHeaders()
{
	std::cout << RED "****The headers map:\n";
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

void HTTPRequest::displayResponse(int fd)
{

	std::cout << MAGENTA "Respond: " << _responses[fd] << RESET <<std::endl;
}

int HTTPRequest::validate()
{
	
	if (_requestMap.find("Host") == _requestMap.end())
		return 400;
	if (_requestMap.find("If-None-Match") != _requestMap.end())
		return 304;
	return 200;
}

std::string HTTPRequest::getResponse()
{
	int statusCode = validate();

	std::cout << RED "****received method is: " BLUE << _method << RESET << std::endl;
	if (statusCode == 400)
	{
		return httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
	}
	if (statusCode == 304)
	{
		return httpStatusCode(304);
	}

	if (_method == "GET" || _method == "HEAD")
	{
		return handleGet();
	}
	else if (_method == "POST")
	{
		return handlePost();
	}
	else if (_method == "DELETE")
	{
		return handleDelete();
	}
	return httpStatusCode(405) + "Content-Type: text/html\r\n\r\n<html><body><h1>Method Not Allowed</h1></body></html>";
}

std::string HTTPRequest::handleGet()
{

	std::string readHtml = readHtmlFile("./src/index.html");
	std::ostringstream reponseHeaders;
	std::string date, lastMfd, eTag;

	if (_uri == "/" || _uri == "/index.html" || _uri == "/favicon.ico")
	{
		eTag = generateETag("./src/index.html", date, lastMfd);
		std::cout << RED "inside Get ......." RESET << std::endl;
		reponseHeaders << httpStatusCode(200);
		reponseHeaders << "Server: " + _serverConfig["server_name"] << CRLF;
		reponseHeaders << "Date: " + date << CRLF;
		reponseHeaders << "Content-Type: text/html\r\n";
		reponseHeaders << "Content-Length: " << readHtml.size() << CRLF;
		reponseHeaders << "Last-Modified: " + lastMfd << CRLF;
		reponseHeaders << "Connection: close" << CRLF;
		reponseHeaders << "ETag: " + eTag << CRLF;
		reponseHeaders << "Cache-Control: no-cache" << CRLF;
		reponseHeaders << "Accept-Ranges: bytes" CRLF CRLF;
		reponseHeaders << CRLF;
		reponseHeaders << readHtml;
		return reponseHeaders.str();
	}
	else
	{
		// return httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
		std::ostringstream errorHeader;
		errorHeader.clear();
		errorHeader << httpStatusCode(404);
		errorHeader << "Connection: close" << CRLF;
		// errorHeader << CR NL;
		return errorHeader.str() + "Content-Type: text/html\r\n\r\n<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></body></html>";
	}
}

std::string HTTPRequest::handlePost()
{
	// Placeholder implementation
	return httpStatusCode(200) + "Connection: close\r\nContent-Type: text/html\r\n\r\n<html><body><h1>POST Request Received</h1></body></html>";
}

std::string HTTPRequest::handleDelete()
{
	// Placeholder implementation
	return httpStatusCode(200) + "Connection: close\r\nContent-Type: text/html\r\n\r\n<html><body><h1>DELETE Request Received</h1></body></html>";
}

std::string HTTPRequest::httpStatusCode(int statusCode)
{
	switch (statusCode)
	{
	case 200:
		return "HTTP/1.1 200 OK\r\n";
	case 304:
		return "HTTP/1.1 304 Not Modified\r\n";
	case 400:
		return "HTTP/1.1 400 Bad Request\r\n";
	case 404:
		return "HTTP/1.1 404 Not Found\r\n";
	case 405:
		return "HTTP/1.1 405 Method Not Allowed\r\n";
	default:
		return "HTTP/1.1 500 Internal Server Error\r\n";
	}
}

bool HTTPRequest::handleRequest(int clientSocket)
{
	char buffer[2048];
	ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

	std::cout << CYAN << "bytesRead = " << bytesRead << RESET << std::endl;
	if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){
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

	//****************print request***********************
	displayRequest();
	//****************************************************

	if (!parse())
	{
		std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
		send(clientSocket, response.c_str(), response.length(), 0);
		close(clientSocket);
		return (false);
		// } else {
		//     std::string response = getResponse();
		//     send(clientSocket, response.c_str(), response.length(), 0);
	}
	return (true);
}

void HTTPRequest::printStringToFile(std::string const & string,std::string const path)
{
	std::cout << RED "****Printing response in file: " BLUE << path << RESET << std::endl;
	std::ofstream outfile(path.c_str());
	outfile << string << std::endl;
	outfile.close();
}


bool HTTPRequest::handleRespons(int clientSocket, int const &pollEvent)
{
	if (pollEvent == POLLIN_TMP) {
		this->_responses[clientSocket] = getResponse();
		// std::cout << "***************************************\n";
		// std::cout << CYAN << "pi:" << this->_responses[clientSocket] << RESET << std::endl;
		// std::cout << "***************************************\n";
		std::cout << RED "Handled request on socket fd " RESET << clientSocket << std::endl;
		return (true);
	}
	if (pollEvent == POLLOUT_TMP) {
		// std::cout << "***************************************\n";
		// std::cout << CYAN << "po:"<< this->_responses[clientSocket] << RESET << std::endl;
		// std::cout << "***************************************\n";
		// std::map<int, std::string>::iterator iter = this->_responses.find(clientSocket);
		// std::cout << "***************************************\n";
		// std::cout << CYAN << "iter:"<< iter->second << RESET << std::endl;
		// std::cout << "***************************************\n";
		// // if (iter == this->_responses.end()) {
		// if (!iter->first || iter->second == "") {
		// 	// this->_responses.erase(clientSocket);

		// 	std::cerr << "No response found for socket fd " << clientSocket << std::endl;
		// 	return (false);
		// }
		std::string response = this->_responses[clientSocket];
		// std::string response = iter->second;

		//****************print the provided response in command prompt***********************
		displayResponse(clientSocket);
		//****************print the provided response in file***********************
		printStringToFile(response, "./src/request/response.txt");
		//**************************************************************************
		ssize_t bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
		if (bytesSent == -1) {
			std::cerr << RED << "Sending response failed" << RESET << std::endl;
			return (false);
		}
		int closeResult = close(clientSocket);
		if (closeResult == -1)
		{
			std::cout << RED << "CLOSING FAILED!!!!!!!!!!!!!!!" << RESET << std::endl;
			return (false);
		}
		std::cout << RED << "Socket [" << clientSocket << "] is closed." << RESET << std::endl;
		
		this->_responses.erase(clientSocket);
		return (true);
	}
	return (false);
}


std::string HTTPRequest::readHtmlFile(std::string path)
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
	return ss.str();
}

std::string getCurrentTimeHTTP()
{
	std::time_t currentTime = std::time(NULL);
	return formatTimeHTTP(currentTime);
}

std::string generateETag(const std::string &filePath, std::string &date, std::string &lastmdf)
{
	struct stat fileInfo;

	if (stat(filePath.c_str(), &fileInfo) != 0)
	{
		std::cerr << "Error getting file information: " << filePath << std::endl; // check if the error method is addmited
		return "";
	}

	std::ostringstream etag;
	etag << "\"" << fileInfo.st_mtime << "-" << fileInfo.st_size << "\"";

	date = getCurrentTimeHTTP();
	lastmdf = formatTimeHTTP(fileInfo.st_mtime);

	return etag.str();
}

std::string formatTimeHTTP(std::time_t rawTime)
{

	// Convert it to GMT time
	std::tm *gmTime = std::gmtime(&rawTime);

	// Format the time to HTTP-date format
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmTime);

	return std::string(buffer);
}

void writHtmlFile(std::string request, std::string path)
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
				case '\r':
					outFile << "'\\r'";
					break;
				case '\n':
					outFile << "\\n";
					break;
				case '\t':
					outFile << "\\t";
					break;
				case '\v':
					outFile << "\\v";
					break;
				case '\f':
					outFile << "\\f";
					break;
				case ' ':
					outFile << "' '";
					break;
				default:
					outFile << c;
				}
			}
		}
	}
	outFile.close();
	exit(1);
}

