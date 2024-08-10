/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/10 13:14:30 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"

GetHandler::GetHandler(const std::map<std::string, std::string>& requestMap,
					   const std::map<std::string, std::string>& serverConfig) :
	HTTPResponse(serverConfig) {
	setRequestMapInResponse(requestMap);
}

GetHandler::~GetHandler() {}
// std::string generateGeneralHeaders() {}
std::string GetHandler::GetMethod()
{
	std::ostringstream responseHeaders;
	std::string date, lastMfd, eTag;
	std::string uri = _requestMap["uri"];
	
	if (uri == "/" || uri == "/index.html") 
		uri = "/index.html";
	
	std::string filePath = _serverConfig.at("root") + uri;
	std::cout << ":::::::: ----- " << filePath << std::endl;
	std::string readHtml = readBinaryFile(filePath);

	if (!readHtml.empty())
	{
		std::string extension = filePath.substr(filePath.find_last_of("."));
		if (extension == CGI)
			readHtml = cgi(uri);
		std::string mimeType = getMimeType(extension);
		eTag = generateETag(filePath, date, lastMfd);

		responseHeaders	<< httpStatusCode(200)
						<< "Date: " 					<< date << CRLF
						<< "Server: " 					<< _serverConfig["server_name"] << CRLF
						<< "Last-Modified: " 			<< lastMfd << CRLF
						<< "ETag: " 					<< eTag << CRLF
						<< "Content-Length: " 			<< readHtml.size() << CRLF
						<< "Cache-Control: no-cache"	<< CRLF
						<< "Content-Type: "				<< mimeType << CRLF
						<< "Connection: keep-alive" 	<< CRLF << CRLF;
		if (_requestMap["method"] == "HEAD") {
			return responseHeaders.str();
		}
		responseHeaders << readHtml;
		return responseHeaders.str();
	// }
	// else if (uri == "/cgi-bin/myScript.sh") // need to be dynamical
	// {
	// 	// std::string filePath = _serverConfig.at("root") + uri;
	// 	std::string extension = filePath.substr(filePath.find_last_of("."));
	// 	std::string mimeType = getMimeType(extension);
	// 	std::string cgiResult = cgi(uri); // ---- nico
	// 	// if (cgiResult.empty()) { // an error occurred

	// 	// }
	// 	eTag = generateETag(uri, date, lastMfd);

	// 	responseHeaders	<< httpStatusCode(200)
	// 					<< "Date: " 					<< date << CRLF
	// 					<< "Server: " 					<< _serverConfig["server_name"] << CRLF
	// 					<< "Last-Modified: " 			<< lastMfd << CRLF
	// 					<< "ETag: " 					<< eTag << CRLF
	// 					<< "Content-Length: " 			<< cgiResult.size() << CRLF
	// 					<< "Cache-Control: no-cache" 	<< CRLF
	// 					<< "Content-Type: "				<< mimeType 		<< CRLF
	// 					<< "Connection: keep-alive" 	<< CRLF << CRLF;
	// 	responseHeaders << cgiResult;
	// 	return responseHeaders.str();
	// }
	// else if (uri == "/favicon.ico")
	// {
	// 	std::string extension = filePath.substr(filePath.find_last_of("."));
	// 	std::string mimeType = getMimeType(extension);
	// 	std::string favIconSize = readHtmlFile(filePath);
	// 	std::string favIcon = readBinaryFile(filePath);
	// 	std::cout << MAGENTA "size: "<< favIcon.size() << "binary size: "<< favIconSize.size() << std::endl;
	// 	eTag = generateETag(uri, date, lastMfd);

	// 	responseHeaders	<< httpStatusCode(200) 
	// 					<< "Date: " 					<< date << CRLF
	// 					<< "Server: " 					<< _serverConfig.at("server_name") << CRLF
	// 					<< "Last-Modified: " 			<< lastMfd << CRLF
	// 					<< "ETag: " 					<< eTag << CRLF
	// 					<< "Content-Length: " 			<< favIcon.size() << CRLF
	// 					<< "Cache-Control: no-cache" 	<< CRLF
	// 					<< "Content-Type: "				<< mimeType 		<< CRLF
	// 					<< "Connection: keep-alive" 	<< CRLF << CRLF;
	// 	responseHeaders	<< favIcon;
	// 	return responseHeaders.str();
	} else
		return httpStatusCode(404) + "Content-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
}

						// << "Content-Type: image/vnd.microsoft.icon" << CRLF