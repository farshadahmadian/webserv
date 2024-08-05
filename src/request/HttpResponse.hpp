/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:45:08 by nnavidd           #+#    #+#             */
/*   Updated: 2024/08/05 10:10:22 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# define CR    "\r"
# define LF    "\n"
# define CRLF  "\r\n"

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
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
    bool handleResponse(int clientSocket, int const &pollEvent); //
    void displayResponse(int fd);
    void setRequestMapInResponse(std::map<std::string, std::string> const & requestMap);
    void setRequestStringInResponse(std::string const & requestString);

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

	// nico
	std::string cgi( std::string& uri );
	char** createEnv( void );

    std::string getCurrentTime();
    std::string formatTimeHTTP(std::time_t rawTime);

    std::map<std::string, std::string> _serverConfig; //-------------------------------------------------Keep A Reference Of Server Config Map
    std::map<std::string, std::string> _requestMap; //---------------------------------------------------Keep A Reference Of Request Map
	std::string	_requestString; //-----------------------------------------------------------------------Keep A Reference Of Request String
    std::map<int, std::string> _responses; //------------------------------------------------------------A Map Of Responses Corresponding To Their Sockets

	private:
		Post _post;
};

#endif // HTTPRESPONSE_HPP


// curl -o file url-to-favicon
// cmp file /path/to/original
