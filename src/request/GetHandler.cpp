/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/03 09:43:58 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"

GetHandler::GetHandler(const std::map<std::string, std::string>& requestMap,
					   const std::map<std::string, std::string>& serverConfig) :
	HTTPResponse(serverConfig) {
	setResponseRequestMap(requestMap);
}

GetHandler::~GetHandler() {}

std::string GetHandler::GetMethod() 
{
	std::string readHtml = readHtmlFile("./src/index.html");
	std::ostringstream reponseHeaders;
	std::string date, lastMfd, eTag;

	std::string uri = _requestMap["uri"];
	if (uri == "/" || uri == "/index.html") 
	{
		eTag = generateETag("./src/index.html", date, lastMfd);

		reponseHeaders	<< httpStatusCode(200) << "Date: " << date << CRLF
						<< "Server: " << _serverConfig["server_name"] << CRLF
						<< "Last-Modified: " << lastMfd << CRLF
						<< "ETag: " << eTag << CRLF
						<< "Content-Length: " << readHtml.size() << CRLF
						<< "Cache-Control: no-cache" << CRLF
						<< "Content-Type: text/html" << CRLF
						<< "Connection: keep-alive" << CRLF << CRLF;
		if (_requestMap["method"] == "HEAD") {
			return reponseHeaders.str();
		}
		reponseHeaders << readHtml;
		return reponseHeaders.str();
	} else if (_requestMap["uri"] == "/favicon.ico") 
	{
		std::string faviconsize = readHtmlFile("./src/favicon.ico");
		std::string favicon = readBinaryFile("./src/favicon.ico");
		eTag = generateETag("./src/favicon.ico", date, lastMfd);

		reponseHeaders	<< httpStatusCode(200) << "Date: " << date << CRLF
						<< "Server: " << _serverConfig.at("server_name") << CRLF
						<< "Last-Modified: " << lastMfd << CRLF
						<< "ETag: " << eTag << CRLF
						<< "Content-Length: " << faviconsize.size() << CRLF
						<< "Cache-Control: no-cache" << CRLF
						<< "Content-Type: image/vnd.microsoft.icon" << CRLF
						<< "Connection: keep-alive" << CRLF << CRLF;
		reponseHeaders	<< favicon;
		return reponseHeaders.str();
	} else
		return httpStatusCode(404) + "Content-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
}

// std::string GetHandler::GetMethod() {
//     std::ostringstream responseHeaders;
//     std::string date, lastMdf, eTag;

//     if (_requestMap["uri"] == "/" || _requestMap["uri"] == "/index.html") {
//         std::string readHtml = readHtmlFile("./src/index.html");
//         eTag = generateETag("./src/index.html", date, lastMdf);

//         responseHeaders << httpStatusCode(200)
//                         << "Server: " << _serverConfig.at("server_name") << CRLF
//                         << "Date: " << date << CRLF
//                         << "Content-Type: text/html" << CRLF
//                         << "Content-Length: " << readHtml.size() << CRLF
//                         << "Last-Modified: " << lastMdf << CRLF
//                         << "Connection: close" << CRLF
//                         << "ETag: " << eTag << CRLF
//                         << "Cache-Control: no-cache" << CRLF
//                         << "Accept-Ranges: bytes" << CRLF << CRLF
//                         << readHtml;
//     std::cout << "HIIIIIIIIIIIIIIIIIII\n";
//         return responseHeaders.str();
//     } else if (_requestMap["uri"] == "/favicon.ico") {
//         std::string favicon = readBinaryFile("./src/request/favicon.ico");
//         eTag = generateETag("./src/request/favicon.ico", date, lastMdf);

//         responseHeaders << httpStatusCode(200)
//                         << "Server: " << _serverConfig.at("server_name") << CRLF
//                         << "Date: " << date << CRLF
//                         << "Content-Type: image/vnd.microsoft.icon" << CRLF
//                         << "Content-Length: " << favicon.size() << CRLF
//                         << "Last-Modified: " << lastMdf << CRLF
//                         << "Connection: close" << CRLF
//                         << "ETag: " << eTag << CRLF
//                         << "Cache-Control: no-cache" << CRLF
//                         << "Accept-Ranges: bytes" << CRLF << CRLF;
//         responseHeaders << favicon;
//         return responseHeaders.str();
//     } else {
//         return httpStatusCode(404) + "Content-Type: text/html\r\n\r\n"
//                "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></body></html>";
//     }
// }

// std::string GetHandler::createHandleGet() {
//     return GetMethod();
// }