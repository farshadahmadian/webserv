/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:46:45 by nnavidd           #+#    #+#             */
/*   Updated: 2024/07/29 19:09:12 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./HttpResponse.hpp"

HTTPResponse::HTTPResponse() {
    // Constructor body (if necessary)
    std::cout << CYAN "HTTPResponse constructor called\n" RESET;
}

HTTPResponse::~HTTPResponse() {
    std::cout << CYAN "HTTPResponse destructor called\n" RESET;
}

HTTPResponse::HTTPResponse(std::map<std::string, std::string> &serverConfig) :
    _serverConfig(serverConfig) {
    std::cout << CYAN "HTTPResponse args constructor called\n" RESET;
}

int HTTPResponse::validate() {
    if (_requestMap.find("Host") == _requestMap.end()) {
        return 400;
    }
    // if (_requestMap.find("If-None-Match") != _requestMap.end()) {
    //     return 304;
    // }
    return 200;
}

std::string HTTPResponse::getResponse() {
    int statusCode = validate();

    std::string method = _requestMap["method"];
    std::cout << RED "****received method is: " BLUE << method << RESET << std::endl;
    if (statusCode == 400) {
        return httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
    }
    if (statusCode == 304) {
        return httpStatusCode(304);
    }

    if (method == "GET" || method == "HEAD") {
        return handleGet();
    } else if (method == "POST") {
        return handlePost();
    } else if (method == "DELETE") {
        return handleDelete();
    }
    return httpStatusCode(405) + "Content-Type: text/html\r\n\r\n<html><body><h1>Method Not Allowed</h1></body></html>";
}

std::string HTTPResponse::handleGet()
{
    std::string readHtml = readHtmlFile("./src/index.html");
    std::ostringstream reponseHeaders;
    std::string date, lastMfd, eTag;

    std::string uri = _requestMap["uri"];
   	if (uri == "/" || uri == "/index.html" || uri == "/favicon.ico")
	{
		eTag = generateETag("./src/index.html", date, lastMfd);

		reponseHeaders	<< httpStatusCode(200)
						<< "Server: " << _serverConfig["server_name"] << CRLF
						<< "Date: " << date << CRLF
						<< "Content-Type: text/html\r\n"
						<< "Content-Length: " << readHtml.size() << CRLF
						<< "Last-Modified: " << lastMfd << CRLF
						<< "Connection: keep-alive" << CRLF
						<< "ETag: " << eTag << CRLF
						<< "Cache-Control: no-cache" << CRLF
						<< "Accept-Ranges: bytes" CRLF << CRLF;
		if (_requestMap["method"] == "HEAD") {
            return reponseHeaders.str();
		}
		reponseHeaders << readHtml;
		return reponseHeaders.str();
    }

    return httpStatusCode(404) + "Content-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
}

std::string HTTPResponse::handlePost() {
    std::string responseBody = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n<html><body><h1>POST Request Received</h1></body></html>";
    return responseBody;
}

std::string HTTPResponse::handleDelete() {
    std::string responseBody = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n<html><body><h1>DELETE Request Received</h1></body></html>";
    return responseBody;
}

std::string HTTPResponse::httpStatusCode(int statusCode) {
    switch (statusCode) {
        case 200: return "HTTP/1.1 200 OK\r\n";
        case 400: return "HTTP/1.1 400 Bad Request\r\n";
        case 404: return "HTTP/1.1 404 Not Found\r\n";
        case 304: return "HTTP/1.1 304 Not Modified\r\n";
        case 405: return "HTTP/1.1 405 Method Not Allowed\r\n";
        default:  return "HTTP/1.1 500 Internal Server Error\r\n";
    }
}

std::string HTTPResponse::readBinaryFile(std::string const & path) {
    std::ifstream fileStream(path.c_str(), std::ios::binary);
    if (!fileStream.is_open()) {
        perror("error:");
        return "";
    }
    std::ostringstream ss;
    ss << fileStream.rdbuf();
    return ss.str();
}

std::string HTTPResponse::readHtmlFile(const std::string &path) {
    std::ifstream fileStream(path.c_str());
    if (!fileStream.is_open()) {
        perror("error:");
        return "";
    }
    std::ostringstream ss;
    ss << fileStream.rdbuf();
    return ss.str();
}

std::string HTTPResponse::formatTimeHTTP(std::time_t rawTime) {
    std::tm *gmTime = std::gmtime(&rawTime);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmTime);
    return std::string(buffer);
}

std::string HTTPResponse::getCurrentTimeHTTP() {
    std::time_t currentTime = std::time(NULL);
    return formatTimeHTTP(currentTime);
}

std::string HTTPResponse::generateETag(const std::string &filePath, std::string &date, std::string &lastmdf) {
    struct stat fileInfo;

    if (stat(filePath.c_str(), &fileInfo) != 0) {
        std::cerr << "Error getting file information: " << filePath << std::endl;
        return "";
    }

    std::ostringstream etag;
    etag << "\"" << fileInfo.st_mtime << "-" << fileInfo.st_size << "\"";

    date = getCurrentTimeHTTP();
    lastmdf = formatTimeHTTP(fileInfo.st_mtime);

    return etag.str();
}


bool HTTPResponse::handleRespons(int clientSocket, int const &pollEvent) {
    if (pollEvent == POLLIN_TMP) {
        _responses[clientSocket] = getResponse();
        std::cout << RED "Handled request on socket fd " RESET << clientSocket << std::endl;
        return true;
    }
    if (pollEvent == POLLOUT_TMP) {
        std::map<int, std::string>::iterator iter = _responses.find(clientSocket);
        if (iter == _responses.end()) {
            std::cerr << "No response found for socket fd " << clientSocket << std::endl;
            return false;
        }
        std::string response = iter->second;

		//****************print the provided response in command prompt***********************
		displayResponse(clientSocket);
		//****************print the provided response in file***********************
		printStringToFile(response, "./src/request/response.txt");
		//**************************************************************************

        ssize_t bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
        if (bytesSent == -1) {
            std::cerr << RED << "Sending response failed" << RESET << std::endl;
            return false;
        }
        _responses.erase(clientSocket);
        return true;
    }
    return false;
}

void HTTPResponse::displayResponse(int fd) {
    std::cout << ORG "****Response for fd [" << fd << "] is:\n" << MAGENTA << _responses[fd] << RESET << std::endl;
}

void HTTPResponse::printStringToFile(std::string const & string,std::string const path)
{
	std::cout << RED "****Printing response in file: " BLUE << path << RESET << std::endl;
	std::ofstream outfile(path.c_str());
	outfile << string << std::endl;
	outfile.close();
}

void HTTPResponse::setResponseRequestMap(std::map<std::string, std::string> & requestMap) {
	_requestMap = requestMap;
}