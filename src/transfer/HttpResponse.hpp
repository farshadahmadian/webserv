/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:45:08 by nnavidd           #+#    #+#             */
/*   Updated: 2024/08/07 16:29:14 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# define CR   	"\r"
# define LF   	"\n"
# define CRLF 	"\r\n"
# define MIME	"./src/transfer/MIME.type"

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <map>
# include <ctime>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <cerrno>
# include <poll.h>
# include "colors.h"
# include "errors.h"
# include "Post.hpp"
# include "ConnectedSocket.hpp"


enum POLLEvents {
	POLLIN_TMP  = 0,
	POLLOUT_TMP = 1,
};

class GetHandler; // Forward declaration

class HTTPResponse {
public:
	HTTPResponse();
	HTTPResponse(std::map<std::string, std::string> const & serverConfig);
	virtual ~HTTPResponse();

	std::string getResponse(int const clientSocket); //---------------------------------------------------Return Error Or Call The Related HTTP Methods
	bool handleResponse(int clientSocket, int const &pollEvent, pollfd *pollFds, size_t i, ConnectedSocket &connectedSocket); //
	void displayResponse(int fd);
	void setRequestMapInResponse(std::map<std::string, std::string> const & requestMap);
	void setRequestStringInResponse(std::string const & requestString);
    void displayRequestMap();
	void displayServerConfig();
	//-------------------------------MIME------------------------------
    std::string getMimeType(const std::string& extension) const;

protected:
	std::string createHandleGet();
	std::string createHandlePost(int const clientSocket);
	std::string createHandleDelete();
	std::string httpStatusCode(int statusCode);
	std::string readBinaryFile(const std::string& path);
	std::string readHtmlFile(const std::string& path);
	std::string generateETag(const std::string& filePath, std::string& date, std::string& lastModified);
	void printStringToFile(const std::string& string, const std::string& path);
	int validate();

	std::string generateGeneralHeaders(std::string & filePath);

	//-------------------------------CGI-------------------------------
	// nico
	std::string cgi( std::string& uri );
	char** createEnv( void );

	std::string getCurrentTime();
	std::string formatTimeHTTP(std::time_t rawTime);
	//-----------------------------------------------------------------


	std::map<std::string, std::string> _serverConfig; //-------------------------------------------------Keep A Reference Of Server Config Map
	std::map<std::string, std::string> _requestMap; //---------------------------------------------------Keep A Reference Of Request Map
	std::string	_requestString; //-----------------------------------------------------------------------Keep A Reference Of Request String
	std::map<int, std::string> _responses; //------------------------------------------------------------A Map Of Responses Corresponding To Their Sockets
	//-------------------------------MIME------------------------------
    std::map<std::string, std::string> _mimeMap;

    void loadMimeTypes(const std::string& filePath);
	//-----------------------------------------------------------------
	private:
		Post _post;
};

#endif // HTTPRESPONSE_HPP



// curl -o file url-to-favicon
// cmp file /path/to/original


/*

	   Response      = Status-Line               ; Section 6.1
					   *(( general-header        ; Section 4.5
						| response-header        ; Section 6.2
						| entity-header ) CRLF)  ; Section 7.1
					   CRLF
					   [ message-body ]          ; Section 7.2

	   			Status-Line 	= HTTP-Version SP Status-Code SP Reason-Phrase CRLF
								The first line of a Response message is the Status-Line, consisting
   								of the protocol version followed by a numeric status code and its
   								associated textual phrase, with each element separated by SP
   								characters. No CR or LF is allowed except in the final CRLF sequence.

				general-header = Cache-Control             ; Section 14.9
								| Connection               ; Section 14.10
								| Date                     ; Section 14.18
								| Pragma                   ; Section 14.32
								| Trailer                  ; Section 14.40
								| Transfer-Encoding        ; Section 14.41
								| Upgrade                  ; Section 14.42
								| Via                      ; Section 14.45
								| Warning                  ; Section 14.46

				response-header = Accept-Ranges           ; Section 14.5
								| Age                     ; Section 14.6
								| ETag                    ; Section 14.19
								| Location                ; Section 14.30
								| Proxy-Authenticate      ; Section 14.33
								| Retry-After             ; Section 14.37
								| Server                  ; Section 14.38
								| Vary                    ; Section 14.44
								| WWW-Authenticate        ; Section 14.47

				entity-header  = Allow                     ; Section 14.7
								| Content-Encoding         ; Section 14.11
								| Content-Language         ; Section 14.12
								| Content-Length           ; Section 14.13
								| Content-Location         ; Section 14.14
								| Content-MD5              ; Section 14.15
								| Content-Range            ; Section 14.16
								| Content-Type             ; Section 14.17
								| Expires                  ; Section 14.21
								| Last-Modified            ; Section 14.29
								| extension-header

								  extension-header = message-header

				[ message-body ]:
								The entity-body (if any) sent with an HTTP request or response is in
								a format and encoding defined by the entity-header fields.

								entity-body    = *OCTET

								An entity-body is only present in a message when a message-body is
								present, as described in section 4.3. The entity-body is obtained
								from the message-body by decoding any Transfer-Encoding that might
								have been applied to ensure safe and proper transfer of the message.

*/