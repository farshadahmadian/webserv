/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:33:30 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/08 10:51:46 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// HTTP status codes
const std::string HTTP_200 = "HTTP/1.1 200 OK\r\n";
const std::string HTTP_400 = "HTTP/1.1 400 Bad Request\r\n";
const std::string HTTP_404 = "HTTP/1.1 404 Not Found\r\n";


class HTTPRequest {
public:
    HTTPRequest(const std::string& request);
    bool parse();
    int validate();
    std::string getResponse();

private:
    std::string request;
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;

    bool isValidMethod(const std::string& method);
    bool isValidHttpVersion(const std::string& version);
};

#endif
