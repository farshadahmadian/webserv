/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:33:30 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/28 11:10:37 by nnabaeei         ###   ########.fr       */
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

# define CR	"\r"
# define LF	"\n"
# define CRLF "\r\n"

// # include "../../include/colors.h"
# include "../../include/errors.h"
# include "../exception/Exception.hpp"
# include "../parsing/HttpConf.hpp"

#include <string>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <unistd.h>    // For read, write, close
#include <iostream>    // For std::cout, std::endl
#include <cstdio>      // For perror
#include <cstring>     // For memset
#include <sstream>     // For std::istringstream
#include <algorithm>   // For std::remove


enum POLLEvents {
POLLIN_TMP  = 0,
POLLOUT_TMP = 1,
};

class HTTPRequest {
public:
	HTTPRequest( void );
	~HTTPRequest( void );
	HTTPRequest(std::map<std::string, std::string> &serverConfig);
	bool parse();
	int validate();
	std::string getResponse();
	bool handleRequest(int clientSocket);
	bool handleRespons(int clientSocket, int const &pollEvent);
	std::string readHtmlFile(std::string path);
	std::map<std::string, std::string> const & getRequest();
	void printStringToFile(std::string const & string, std::string const path);
	void displayRequest() const;
	void displayHeaders();
	void displayServerConfig();
	void displayResponse(int fd);
	

private:
	std::string _requestString;
	std::string _method;
	std::string _uri;
	std::string _version;
    std::string _body;
	std::map<int, std::string> _responses;
	std::map<std::string, std::string> _requestMap;
	std::map<std::string, std::string> _serverConfig;


    std::string handleGet();
    std::string handlePost();
    std::string handleDelete();

	bool isValidMethod(const std::string& method);
	bool isValidHttpVersion(const std::string& version);
	std::string  httpStatusCode(int code);
};

std::string generateETag(const std::string& filePath, std::string &date, std::string &lastmdf);
std::string formatTimeHTTP(std::time_t rawTime);
void	writHtmlFile(std::string request, std::string path);
#endif



