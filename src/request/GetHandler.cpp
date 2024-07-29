/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/29 22:47:28 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./GetHandler.hpp"

GetHandler::GetHandler() {}

GetHandler::~GetHandler() {}

std::string GethHandler::GetMethod() {
    std::ostringstream reponseHeaders;
    std::string date, lastMfd, eTag;

    if (_requestMap["uri"] == "/" || _requestMap["uri"] == "/index.html") {
        std::string readHtml = readHtmlFile("./src/index.html");
        eTag = generateETag("./src/index.html", date, lastMfd);

        reponseHeaders << httpStatusCode(200)
                       << "Server: " << _serverConfig["server_name"] << CRLF
                       << "Date: " << date << CRLF
                       << "Content-Type: text/html" << CRLF
                       << "Content-Length: " << readHtml.size() << CRLF
                       << "Last-Modified: " << lastMfd << CRLF
                       << "Connection: close" << CRLF
                       << "ETag: " << eTag << CRLF
                       << "Cache-Control: no-cache" << CRLF
                       << "Accept-Ranges: bytes" << CRLF << CRLF
                       << readHtml;
        return reponseHeaders.str();
    } else if (_requestMap["uri"] == "/favicon.ico") {
        std::string favicon = readBinaryFile("./src/request/favicon.ico");
        eTag = generateETag("./src/request/favicon.ico", date, lastMfd);

        reponseHeaders << httpStatusCode(200)
                       << "Server: " << _serverConfig["server_name"] << CRLF
                       << "Date: " << date << CRLF
                       << "Content-Type: image/vnd.microsoft.icon" << CRLF
                       << "Content-Length: " << favicon.size() << CRLF
                       << "Last-Modified: " << lastMfd << CRLF
                       << "Connection: close" << CRLF
                       << "ETag: " << eTag << CRLF
                       << "Cache-Control: no-cache" << CRLF
                       << "Accept-Ranges: bytes" << CRLF << CRLF;
        reponseHeaders << favicon;
        return reponseHeaders.str();
    } else {
        return httpStatusCode(404) + "Content-Type: text/html\r\n\r\n"
               "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></body></html>";
    }
}