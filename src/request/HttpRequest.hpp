/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:33:30 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/15 16:37:38 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// # define BLUE		"\033[38;5;4m"
// # define MAGENTA	"\033[38;5;5m"
// # define CYAN		"\033[38;5;44m"
// # define GREEN		"\033[38;5;2m"
// # define ORG		"\033[38;5;214m"
// # define RED		"\033[38;5;196m"
// # define RESET		"\033[0m"

# include "colors.h"
# include "errors.h"
# include "../exception/Exception.hpp"

#include <string>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>    // For read, write, close
#include <iostream>    // For std::cout, std::endl
#include <cstdio>      // For perror
#include <cstring>     // For memset
#include <sstream>     // For std::istringstream
#include <algorithm>   // For std::remove


class HTTPRequest {
public:
	HTTPRequest(const std::string& request);
	bool parse();
	int validate();
	std::string getResponse();
	void handleRequest(int clientSocket);

private:
	std::string _request;
	std::string _method;
	std::string _uri;
	std::string _version;
	std::map<std::string, std::string> _headers;
    std::string _body;

    std::string handleGet();
    std::string handlePost();
    std::string handleDelete();

	bool isValidMethod(const std::string& method);
	bool isValidHttpVersion(const std::string& version);
	std::string  httpStatusCode(int code);
};

#endif



