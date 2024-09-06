/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:33:30 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/09/06 14:06:35 by nnabaeei         ###   ########.fr       */
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
#include <poll.h>
#include "ConnectedSocket.hpp"

class HTTPRequest {
public:
	HTTPRequest( void );
	~HTTPRequest( void );
	HTTPRequest(std::map<std::string, std::string> const & serverConfig);

	bool parse(ConnectedSocket &connectedSocket); //------------------------------------------------Parse The Received Request and Create a Map
	bool handleRequest(int connectedSocketFd, pollfd *pollFds, size_t i, ConnectedSocket &connectedSocket); //------------------------Receive The Request From The Socket
	std::map<std::string, std::string> const & getRequestMap();
	std::string const & getRequestString() const;
	void setServerConfig(std::map<std::string, std::string> const & serverConfig);
	// bool parse();
	// int validate();
	std::map<std::string, std::string> &getServerConfig(void);

	void displayRequestString() const;
	void printRequestMap();
	void printServerConfig();
	bool receiveInChuncks(ConnectedSocket &connectedSocket, pollfd *pollFds, size_t i, std::ostringstream const &outputStringStream);
	void readAllHeader(ConnectedSocket &connectedSocket, pollfd *pollFds, size_t i);
	void storeHeader(ConnectedSocket &connectedSocket);
	bool isHeaderReceived(std::string buffer);
	void readAllBody(ConnectedSocket &connectedSocket, pollfd *pollFds, size_t i, std::ostringstream const &outputStringStream);
	std::string extractContentLength(std::string request);
	std::string extractBody(std::string request);
	std::string extractHeader(std::string request);


private:
	std::string _requestString; //----------------------------------Keep The whole Request String
	std::string _method;
	std::string _uri;
	std::string _version;
	std::map<std::string, std::string> _requestMap; //--------------Keep a Map of Request
	std::map<std::string, std::string> _serverConfig; //------------Keep a Map of Server_Config

	bool isValidMethod(const std::string& method); //---------------Check The Method of The Received Request
	bool isValidHttpVersion(const std::string& version); //---------Check The Received Request HTTP Version validation
	bool isCgiRequest( void );
};

void	writeStringToFile(std::string string, std::string filepath);
#endif

/*
		Request     = Request-Line            							; Section 5.1
					*(( general-header        							; Section 4.5
					| request-header         							; Section 5.3
					| entity-header ) CRLF)  							; Section 7.1
					CRLF
					[ message-body ]

				Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
								The Request-Line begins with a method token, followed by the
								Request-URI and the protocol version, and ending with CRLF. The
								elements are separated by SP characters. No CR or LF is allowed
								except in the final CRLF sequence.


				general-header = Cache-Control             				; Section 14.9
								| Connection               				; Section 14.10
								| Date                     				; Section 14.18
								| Pragma                   				; Section 14.32
								| Trailer                  				; Section 14.40
								| Transfer-Encoding        				; Section 14.41
								| Upgrade                  				; Section 14.42
								| Via                      				; Section 14.45
								| Warning                  				; Section 14.46


				request-header = Accept                    				; Section 14.1
								| Accept-Charset           				; Section 14.2
								| Accept-Encoding          				; Section 14.3
								| Accept-Language          				; Section 14.4
								| Authorization            				; Section 14.8
								| Expect                   				; Section 14.20
								| From                     				; Section 14.22
								| Host                     				; Section 14.23
								| If-Match                 				; Section 14.24
								| If-Modified-Since        				; Section 14.25
								| If-None-Match            				; Section 14.26
								| If-Range                 				; Section 14.27
								| If-Unmodified-Since      				; Section 14.28
								| Max-Forwards             				; Section 14.31
								| Proxy-Authorization      				; Section 14.34
								| Range                    				; Section 14.35
								| Referer                  				; Section 14.36
								| TE                       				; Section 14.39
								| User-Agent               				; Section 14.43

				entity-header  = Allow                     				; Section 14.7
								| Content-Encoding         				; Section 14.11
								| Content-Language         				; Section 14.12
								| Content-Length           				; Section 14.13
								| Content-Location         				; Section 14.14
								| Content-MD5              				; Section 14.15
								| Content-Range            				; Section 14.16
								| Content-Type             				; Section 14.17
								| Expires                  				; Section 14.21
								| Last-Modified            				; Section 14.29
								| extension-header

								  extension-header = message-header

				[ message-body ]:

								7.2 Entity Body
								The entity-body (if any) sent with an HTTP request or response is in
								a format and encoding defined by the entity-header fields.

								entity-body    = *OCTET

								An entity-body is only present in a message when a message-body is
								present, as described in section 4.3. The entity-body is obtained
								from the message-body by decoding any Transfer-Encoding that might
								have been applied to ensure safe and proper transfer of the message.

*/