/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:45:08 by nnavidd           #+#    #+#             */
/*   Updated: 2024/07/31 10:16:08 by nnabaeei         ###   ########.fr       */
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

    std::string getResponse(int const clientSocket);
    bool handleRespons(int clientSocket, int const &pollEvent);
    void displayResponse(int fd);
    void setResponseRequestMap(std::map<std::string, std::string> const & requestMap);
    void setResponseRequestString(std::string const & requestString);

protected:
    std::string createHandleGet();
    std::string createHandlePost(int const clinetSocket);
    std::string createHandleDelete();
    std::string httpStatusCode(int statusCode);
    std::string readBinaryFile(const std::string& path);
    std::string readHtmlFile(const std::string& path);
    std::string generateETag(const std::string& filePath, std::string& date, std::string& lastmdf);
    void printStringToFile(const std::string& string, const std::string& path);
    int validate();

    std::string getCurrentTimeHTTP();
    std::string formatTimeHTTP(std::time_t rawTime);

    std::map<std::string, std::string> _serverConfig;
    std::map<std::string, std::string> _requestMap;
	std::string	_requestString;
    std::map<int, std::string> _responses;

	private:
		Post _post;
};

#endif // HTTPRESPONSE_HPP
