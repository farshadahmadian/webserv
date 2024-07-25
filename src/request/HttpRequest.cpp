/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnavidd <nnavidd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:39:02 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/25 19:39:17 by nnavidd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HTTPRequest::HTTPRequest( void ) {}

HTTPRequest::~HTTPRequest( void ) { std::cout << BLUE "HTTPRequest destructor called\n" RESET;}

HTTPRequest::HTTPRequest(std::map<std::string, std::string> &serverConfig) :
	_null(""),
	_request(""),
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
	std::istringstream requestStream(_request);
	std::string line;

	// Parse request line
	if (!std::getline(requestStream, line))
	{
		std::cout << "HIIIIIIIIIIII1111\n";
		return false;
	}
	std::istringstream requestLine(line);
	if (!(requestLine >> _method >> _uri >> _version))
	{
		// std::cout << "HIIIIIIIIII\n";
		return false;
	}

	if (!isValidMethod(_method) || !isValidHttpVersion(_version))
	{
		return false;
	}

	// Parse headers
	_headers["version"] = _version;
	_headers["uri"] = _uri;
	_headers["method"] = _method;

	while (std::getline(requestStream, line) && line != "\r")
	{
		size_t pos = line.find(":");
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
			value.erase(0, value.find_first_not_of(" "));
			_headers[key] = value;
		}
	}

	//****************print header map********************
	displayHeaders();
	//****************print srver config map**************
	displayServerConfig();
	//****************************************************

	return true;
}

std::map<std::string, std::string> const &HTTPRequest::getHeaders() {return (_headers);}

void HTTPRequest::displayRequest() const
{
	std::cout << RED "****received request:\n"
		<< CYAN << _request << RESET << std::endl;
}

void HTTPRequest::displayHeaders()
{
	std::cout << RED "****The headers map:\n";
	std::map<std::string, std::string>::iterator itr = _headers.begin();
	for (; itr != _headers.end(); itr++)
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
	
	if (_headers.find("Host") == _headers.end())
		return 400;
	if (_headers.find("If-None-Match") != _headers.end())
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

	std::string readHtml = readHtmlFile("./src/images/index.html");
	std::ostringstream headers;
	std::string date, lastMfd, eTag;

	if (_uri == "/" || _uri == "/index.html" || _uri == "/favicon.ico")
	{
		eTag = generateETag("./src/index.html", date, lastMfd);
		std::cout << RED "inside Get ......." RESET << std::endl;
		headers << httpStatusCode(200);
		headers << "Server: " + _serverConfig["server_name"] << CRLF;
		headers << "Date: " + date << CRLF;
		headers << "Content-Type: text/html\r\n";
		headers << "Content-Length: " << readHtml.size() << CRLF;
		headers << "Last-Modified: " + lastMfd << CRLF;
		headers << "Connection: close" << CRLF;
		headers << "ETag: " + eTag << CRLF;
		headers << "Accept-Ranges: bytes" CRLF CRLF;
		headers << CRLF;
		headers << readHtml;
		return headers.str();
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
	_request.assign(buffer);

	//****************print request***********************
	displayRequest();
	//****************************************************

	if (!parse())
	{
		
		std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
		send(clientSocket, response.c_str(), response.length(), 0);
		close(clientSocket);
				std::cout << "HIIIIIIIIIII2222\n";

		return (false);
		// } else {
		//     std::string response = getResponse();
		//     send(clientSocket, response.c_str(), response.length(), 0);
	}
	return (true);
}

void HTTPRequest::printStringToFile(std::string const & string,std::string const path)
{
	std::cout << RED "****sending the response\n" RESET;
	std::ofstream outfile(path.c_str());
	outfile << string << std::endl;
	outfile.close();
}


bool HTTPRequest::handleRespons(int clientSocket, int const &pollEvent)
{
	if (pollEvent == POLL_IN) {
		this->_responses[clientSocket] = getResponse();
		std::cout << RED "Handled request on socket fd " RESET << clientSocket << std::endl;
		return (true);
	}
	if (pollEvent == POLL_OUT) {
		std::map<int, std::string>::iterator iter = this->_responses.find(clientSocket);
		if (iter == this->_responses.end()) {
			std::cerr << "No response found for socket fd " << clientSocket << std::endl;
			return (false);
		}
		std::string response = this->_responses[clientSocket];

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

	std::cout << RED "HIIIII\n" RESET;
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

