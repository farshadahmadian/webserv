/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:39:02 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/15 17:18:43 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HTTPRequest::HTTPRequest(const std::string& req) : _request(req) {}

bool HTTPRequest::isValidMethod(const std::string& mthd) {
	return (mthd == "GET" || mthd == "POST" || mthd == "HEAD");
}

bool HTTPRequest::isValidHttpVersion(const std::string& ver) {
	return (ver == "HTTP/1.1" || ver == "HTTP/1.0");
}

bool HTTPRequest::parse() {
	std::istringstream requestStream(_request);
	std::string line;

	// Parse request line
	if (!std::getline(requestStream, line)) {
		return false;
	}
	std::istringstream requestLine(line);
	// std::cout <<  RED"output is: " RESET<< (requestLine >> _method >> _uri >> _version)<< std::endl;
	// std::cout << RED << requestLine << BLUE << _method << RED << _uri << BLUE << _version << RESET<< std::endl;
	if (!(requestLine >> _method >> _uri >> _version)) {
		// std::cout << "HIIIIIIIIII\n";
		return false;
	}

	if (!isValidMethod(_method) || !isValidHttpVersion(_version)) {
		return false;
	}

	// std::cout << RED "HIIIII\n" RESET;
	// std::istringstream streamTest(_request);
	// std::string stringTest;
	//     std::ofstream outFile("navid");
	// while (std::getline(streamTest, stringTest)){
	// 	std::cout  << ORG << stringTest << RESET<< std::endl;

    //     // Manually escape special characters
	// 	if (outFile.is_open()) {
	// 		for (std::string::size_type i = 0; i < stringTest.size(); ++i) {
    //         	char c = stringTest[i];
	// 			switch (c) {
	// 				case '\r':
	// 					outFile << "\\r";
	// 					break;
	// 				case '\n':
	// 					outFile << "\\n";
	// 					break;
	// 				case '\t':
	// 					outFile << "\\t";
	// 					break;
	// 				case '\v':
	// 					outFile << "\\v";
	// 					break;
	// 				case '\f':
	// 					outFile << "\\f";
	// 					break;
	// 				case ' ':
	// 					outFile << "' '";
	// 					break;
	// 				default:
	// 					outFile << c;
	// 			}
	// 		}
	// 	}
	// }
    // outFile.close();
	// exit(1);
	
	
	// Parse headers
	while (std::getline(requestStream, line) && line != "\r") {
		size_t pos = line.find(":");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
			value.erase(0, value.find_first_not_of(" "));
			_headers[key] = value;
		}
	}
	std::map<std::string, std::string>::iterator itr = _headers.begin();
	for(; itr != _headers.end(); itr++)
		std::cout << ORG << itr->first << ":" BLUE << itr->second << RESET << std::endl;


	//parsing the post 
	if (_method == "POST") {
        std::getline(requestStream, _body);
    }

	return true;
}

int HTTPRequest::validate() {
	if (_headers.find("Host") == _headers.end()) {
		return 400;
	}
	return 200;
}

std::string HTTPRequest::getResponse() {
	int statusCode = validate();

    if (statusCode != 200) {
        return httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
    }

    if (_method == "GET") {
        return handleGet();
    } else if (_method == "POST") {
        return handlePost();
    } else if (_method == "DELETE") {
        return handleDelete();
    }

    return httpStatusCode(405) + "Content-Type: text/html\r\n\r\n<html><body><h1>Method Not Allowed</h1></body></html>";

}

std::string HTTPRequest::handleGet() {
    // Placeholder implementation
    return httpStatusCode(200) + "Content-Type: text/html\r\n\r\n<html><body><h1>GET Request Received</h1></body></html>";
}

std::string HTTPRequest::handlePost() {
    // Placeholder implementation
    return httpStatusCode(200) + "Content-Type: text/html\r\n\r\n<html><body><h1>POST Request Received</h1></body></html>";
}

std::string HTTPRequest::handleDelete() {
    // Placeholder implementation
    return httpStatusCode(200) + "Content-Type: text/html\r\n\r\n<html><body><h1>DELETE Request Received</h1></body></html>";
}

std::string HTTPRequest::httpStatusCode(int statusCode) {
    switch (statusCode) {
        case 200: return "HTTP/1.1 200 OK\r\n";
        case 400: return "HTTP/1.1 400 Bad Request\r\n";
        case 404: return "HTTP/1.1 404 Not Found\r\n";
        case 405: return "HTTP/1.1 405 Method Not Allowed\r\n";
        default: return "HTTP/1.1 500 Internal Server Error\r\n";
    }
}

void HTTPRequest::handleRequest(int clientSocket) {
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0) {
		throw Exception("Receive Failed", RECEIVE_FAILED);
        close(clientSocket);
        return;
    }
    buffer[bytesRead] = '\0';
    _request = buffer;
	std::cout << ORG "received request:\n" << CYAN << _request << RESET << std::endl;

    if (!parse()) {
        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
        send(clientSocket, response.c_str(), response.length(), 0);
    } else {
        std::string response = getResponse();
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    close(clientSocket);
}
