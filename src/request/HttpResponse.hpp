/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnavidd <nnavidd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:45:08 by nnavidd           #+#    #+#             */
/*   Updated: 2024/07/29 00:22:40 by nnavidd          ###   ########.fr       */
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

// Define colors for terminal output (if necessary)
// # define BLUE "\033[34m"
// # define RED "\033[31m"
// # define CYAN "\033[36m"
// # define ORG "\033[33m"
// # define MAGENTA "\033[35m"
// # define RESET "\033[0m"
// # define CRLF "\r\n"

class HTTPResponse {
public:
    HTTPResponse();
    ~HTTPResponse();
    HTTPResponse(std::map<std::string, std::string> &serverConfig, std::map<std::string, std::string> &requestMap);

    std::string getResponse();
    bool handleRespons(int clientSocket, int const &pollEvent);
    void displayResponse(int fd);

private:
    std::string handleGet();
    std::string handlePost();
    std::string handleDelete();
    std::string httpStatusCode(int statusCode);
    std::string readHtmlFile(const std::string &path);
    std::string generateETag(const std::string &filePath, std::string &date, std::string &lastmdf);
    int validate();

    std::string getCurrentTimeHTTP();
    std::string formatTimeHTTP(std::time_t rawTime);

    std::map<std::string, std::string> _serverConfig;
    std::map<std::string, std::string> _requestMap;
    std::map<int, std::string> _responses;
};

#endif
