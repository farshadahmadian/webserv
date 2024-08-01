/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:33:30 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/01 11:55:54 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

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

class HTTPRequest {
public:
	HTTPRequest( void );
	~HTTPRequest( void );
	HTTPRequest(std::map<std::string, std::string> const & serverConfig);
	
	bool parse(); //------------------------------------------------Parse The Received Request and Create a Map
	int	validate();
	bool handleRequest(int clientSocket); //------------------------Receive The Request From The Socket
	std::map<std::string, std::string> const & getRequestMap();
	std::string const & getRequestString() const;
	void setServerConfig(std::map<std::string, std::string> const & serverConfig);
	void displayRequestString() const;
	void displayRequestMap();
	void displayServerConfig();
	

private:
	std::string _requestString; //----------------------------------Keep The whole Request String
	std::string _method;
	std::string _uri;
	std::string _version;
	std::map<std::string, std::string> _requestMap; //--------------Keep a Map of Request 
	std::map<std::string, std::string> _serverConfig; //------------Keep a Map of Server_Config

	bool isValidMethod(const std::string& method); //---------------Check The Method of The Received Request
	bool isValidHttpVersion(const std::string& version); //---------Check The Received Request HTTP Version validation
};

void	writeStringToFile(std::string string, std::string filepath);
#endif



