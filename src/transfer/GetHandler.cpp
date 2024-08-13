/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/13 16:04:50 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"

GetHandler::GetHandler(const std::map<std::string, std::string>& requestMap,
					   const std::map<std::string, std::string>& serverConfig) :
	HTTPResponse(serverConfig) {
	setRequestMapInResponse(requestMap);
}

GetHandler::~GetHandler() {}

std::string GetHandler::handleDirectoryListing(const std::string& dirPath) {
	Server::logMessage("INFO: Received GET request for " + dirPath);
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
		Server::logMessage("ERROR: Received GET request for '" + dirPath + "' But There Isn't This Directory!");
        return generateErrorPage(500); // Internal Server Error
    }

    std::string content = "<html><head><title>Index of " + dirPath + 
						"</title></head><body><h1>Index of " + dirPath + "</h1><ul>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // content += "<li><a href=\"" + dirPath + std::string(entry->d_name) + "\">" + dirPath + std::string(entry->d_name) + "</a></li>";
            content += "<li><a href=\"" + _requestMap.at("uri") + "/" + std::string(entry->d_name) + "\">" + dirPath + std::string(entry->d_name) + "</a></li>";
        }
    }
    closedir(dir);

    content += "</ul></body></html>";

    // Now add the HTTP headers
    std::ostringstream responseHeaders;
    responseHeaders << "HTTP/1.1 200 OK\r\n";
    responseHeaders << "Content-Type: text/html\r\n";
    responseHeaders << "Content-Length: " << content.size() << "\r\n";
    responseHeaders << "Connection: keep-alive\r\n";
    responseHeaders << "\r\n"; // End of headers

    // Append the content to the headers
    responseHeaders << content;

    return responseHeaders.str();
}

std::string GetHandler::GetMethod() {
    std::ostringstream responseHeaders;
    std::string date, lastMfd, eTag;
    std::string uri = _requestMap["uri"];
    
    if (uri == "/")
        uri = "/index.html";
    
    std::string filePath = _serverConfig.at("root") + uri;
	Server::logMessage("INFO: Received GET request for " + filePath);
    struct stat st;

    // Check if the requested path is a directory
    if (stat(filePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        // Check for index file
        std::vector<std::string> indexFiles;
		indexFiles.push_back("index.html");
		indexFiles.push_back("index.htm");
		indexFiles.push_back("default.html");
        
		bool indexFileFound = false;


		for (std::vector<std::string>::iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
    		const std::string& indexFile = *it;
            std::string fullPath = filePath + "/" + indexFile;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                filePath = fullPath;
				Server::logMessage("INFO: Index File Found In The " + filePath);
                indexFileFound = true;
                break;
            }
        }

        // If no index file is found and autoindex is enabled, generate directory listing
        if (!indexFileFound) {
            if (_serverConfig["autoindex"] == "on") {
                return handleDirectoryListing(filePath);
            } else {
				Server::logMessage("WARNING: AutoIndex Is Off " + filePath);
                return generateErrorPage(403); // Forbidden
            }
        }
    }

    // Serve the file if it's not a directory or an index file was found
    std::string readHtml = readBinaryFile(filePath);

    if (!readHtml.empty()) {
        std::string extension = filePath.substr(filePath.find_last_of("."));
        if (extension == CGI)
            readHtml = cgi(uri);
        std::string mimeType = getMimeType(extension);
        eTag = generateETag(filePath, date, lastMfd);

        responseHeaders << httpStatusCode(200) << CRLF
                        << "Date: " << date << CRLF
                        << "Server: " << _serverConfig["server_name"] << CRLF
                        << "Last-Modified: " << lastMfd << CRLF
                        << "ETag: " << eTag << CRLF
                        << "Content-Length: " << readHtml.size() << CRLF
                        << "Cache-Control: no-cache" << CRLF
                        << "Content-Type: " << mimeType << CRLF
                        << "Connection: keep-alive" << CRLF << CRLF;

        if (_requestMap["method"] == "HEAD") {
            return responseHeaders.str();
        }

        responseHeaders << readHtml;
        return responseHeaders.str();
    } else {
        return generateErrorPage(404); // Not Found
    }
}


// std::string GetHandler::GetMethod()
// {
// 	std::ostringstream responseHeaders;
// 	std::string date, lastMfd, eTag;
// 	std::string uri = _requestMap["uri"];
	
// 	if (uri == "/" || uri == "/index.html") 
// 		uri = "/index.html";
	
// 	std::string filePath = _serverConfig.at("root") + uri;
// 	std::cout << ":::::::: ----- " << filePath << std::endl;
// 	std::string readHtml = readBinaryFile(filePath);

// 	if (!readHtml.empty())
// 	{
// 		std::string extension = filePath.substr(filePath.find_last_of("."));
// 		if (extension == CGI)
// 			readHtml = cgi(uri);
// 		std::string mimeType = getMimeType(extension);
// 		eTag = generateETag(filePath, date, lastMfd);

// 		responseHeaders	<< httpStatusCode(200) 			<< CRLF
// 						<< "Date: " 					<< date << CRLF
// 						<< "Server: " 					<< _serverConfig["server_name"] << CRLF
// 						<< "Last-Modified: " 			<< lastMfd << CRLF
// 						<< "ETag: " 					<< eTag << CRLF
// 						<< "Content-Length: " 			<< readHtml.size() << CRLF
// 						<< "Cache-Control: no-cache"	<< CRLF
// 						<< "Content-Type: "				<< mimeType << CRLF
// 						<< "Connection: keep-alive" 	<< CRLF << CRLF;
// 		if (_requestMap["method"] == "HEAD") {
// 			return responseHeaders.str();
// 		}
// 		responseHeaders << readHtml;
// 		return responseHeaders.str();
// 	} else
// 		// return httpStatusCode(404) + "Content-Type: text/html\r\nconnection: keep-alive\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
// 		return generateErrorPage(404);
// }
