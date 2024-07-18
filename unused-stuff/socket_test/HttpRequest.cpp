/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:39:02 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/08 10:43:05 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HTTPRequest::HTTPRequest(const std::string& request) : request(request) {}

bool HTTPRequest::isValidMethod(const std::string& method) {
    return (method == "GET" || method == "POST" || method == "HEAD");
}

bool HTTPRequest::isValidHttpVersion(const std::string& version) {
    return (version == "HTTP/1.1" || version == "HTTP/1.0");
}

bool HTTPRequest::parse() {
    std::istringstream requestStream(request);
    std::string line;

    // Parse request line
    if (!std::getline(requestStream, line)) {
        return false;
    }
    std::istringstream requestLine(line);
    if (!(requestLine >> method >> uri >> version)) {
        return false;
    }

    if (!isValidMethod(method) || !isValidHttpVersion(version)) {
        return false;
    }

    // Parse headers
    while (std::getline(requestStream, line) && line != "\r") {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
            value.erase(0, value.find_first_not_of(" "));
            headers[key] = value;
        }
    }

    return true;
}

int HTTPRequest::validate() {
    if (headers.find("Host") == headers.end()) {
        return 400;
    }
    return 200;
}

std::string HTTPRequest::getResponse() {
    int statusCode = validate();

    if (statusCode == 200) {
        return HTTP_200 + "Content-Type: text/html\r\n\r\n<html><body><h1>Success</h1></body></html>";
    } else {
        return HTTP_400 + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
    }
}
