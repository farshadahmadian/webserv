/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:45:08 by nnavidd           #+#    #+#             */
/*   Updated: 2024/07/29 19:09:32 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# define CR	"\r"
# define LF	"\n"
# define CRLF "\r\n"


# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <map>
# include <vector>
# include <ctime>
# include <cstring>
# include <algorithm>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <unistd.h>
# include <cerrno>
# include <poll.h>
# include "colors.h"
# include "error.h"

enum POLLEvents {
POLLIN_TMP  = 0,
POLLOUT_TMP = 1,
};

class HTTPResponse {
public:
    HTTPResponse();
    ~HTTPResponse();
    HTTPResponse(std::map<std::string, std::string> &serverConfig);

    std::string getResponse();
    bool handleRespons(int clientSocket, int const &pollEvent);
    void displayResponse(int fd);
	void setResponseRequestMap(std::map<std::string, std::string> &requestMap);

private:
    std::string handleGet();
    std::string handlePost();
    std::string handleDelete();
    std::string httpStatusCode(int statusCode);
	std::string readBinaryFile(std::string const & path);
    std::string readHtmlFile(const std::string &path);
    std::string generateETag(const std::string &filePath, std::string &date, std::string &lastmdf);
	void printStringToFile(std::string const & string,std::string const path);
    int validate();

    std::string getCurrentTimeHTTP();
    std::string formatTimeHTTP(std::time_t rawTime);

    std::map<std::string, std::string> _serverConfig;
    std::map<std::string, std::string> _requestMap;
    std::map<int, std::string> _responses;
};

#endif
