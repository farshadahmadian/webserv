/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/09/06 19:20:37 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(const std::map<std::string, std::string>& requestMap,
					   const std::map<std::string, std::string>& serverConfig, std::vector<LocationConf> const &locations) :
	HTTPResponse(serverConfig, locations) {
	setRequestMapInResponse(requestMap);
	this->setRedirections();
	this->_cgiDirectory = "/cgi-get/";
	// this->printRedirections();
}

Get::~Get() {}

/*Generate the directory list inside the directory that is passed to it.*/
std::string Get::handleDirectoryListing(const std::string& dirPath) {
	Server::logMessage("INFO: Received GET request for " + dirPath);
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
		Server::logMessage("ERROR: Received GET request for '" + dirPath + "' But There Isn't This Directory!");
        return generateErrorPage(404); // Internal Server Error
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

/*Return the correct address of the index file inside the received directory address
in case if its existence.*/
std::string Get::findIndexFile(const std::string& dirPath) {
	std::string indexes = getLocationIndex(_requestMap["uri"]);
	if (indexes == "")
		indexes = _serverConfig.at("index");
    std::vector<std::string> indexFiles;
	std::istringstream iss(indexes);
	std::string index;

	// printIndexes();
	while (iss >> index) {
    	indexFiles.push_back(index);
	}
    struct stat st;
    for (std::vector<std::string>::iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
        const std::string& indexFile = *it;
        // std::string fullPath = dirPath + "/" + indexFile;
        std::string fullPath = dirPath  + indexFile;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
            Server::logMessage("INFO: Index File Found: " + fullPath);
            return fullPath;
        }
    }
    Server::logMessage("INFO: Index File Not Found For This Address: " + dirPath);

    return ""; // Return empty string if no index file is found
}

std::string const Get::setServerRoot(std::string const & filePath) {
    if (filePath == "./www//farshad/")
        return ("localhost:8080");
    if (filePath == "./www//navid/")
        return ("localhost:8081");
    if (filePath == "./www//nico/")
        return ("localhost:8082");
    return ("localhost:8080");
}


std::string Get::handleGet(ConnectedSocket &connectedSocket) {
	std::string content;
	std::string date, lastMfd, eTag;
	std::ostringstream responseHeaders;
	std::string uri = _requestMap["uri"];
	std::string filePath = _serverConfig.at("root") + uri;


	if (isCgiUri(connectedSocket)) {
		std::string response = this->handleCgi(connectedSocket);
		return response;
	}

	if (getRedirection(uri).length()) {
		std::cout << GREEN << "Redirecting from " << uri << " to " << getRedirection(uri) << RESET << std::endl;
		responseHeaders << httpStatusCode(302) << CRLF
		<< "Location: " << getRedirection(uri) << CRLF
		<< "Connection: keep-alive" << CRLF << CRLF;
		return responseHeaders.str();
	}	

	Server::logMessage("INFO: Received GET request for " + filePath);

    // Check if the requested path is a directory
    if (!isFile(filePath)) {

			if (uri[uri.length() - 1] != '/') {
				responseHeaders << httpStatusCode(302) << CRLF
				<< "Location: " << uri << "/" << CRLF
				<< "Connection: keep-alive" << CRLF << CRLF;
				return responseHeaders.str();
			}
			
			// Check for an index file in the directory
			std::string indexFilePath = findIndexFile(filePath);
			// std::cout << "fullPath outside of findIndexFile: " << indexFilePath << std::endl;
			if (!indexFilePath.empty()) {
					filePath = indexFilePath; // Update filePath to point to the index file
			} else {
					// If no index file is found and autoindex is enabled, generate directory listing
					if (_serverConfig["autoindex"] == "on") {
							return handleDirectoryListing(filePath);
					} else {
							Server::logMessage("WARNING: AutoIndex Is Off for " + filePath);
							return generateErrorPage(403); // Forbidden
					}
			}
	}
    content = readBinaryFile(filePath);
    Server::logMessage("INFO: Received GET request for " + filePath);

    // Serve the file if it's not a directory or an index file was found
    if (!content.empty()){// || isCGI(filePath)) {
        std::string extension = filePath.substr(filePath.find_last_of("."));
		if (extension[extension.size() - 4] != '.')
			extension = HTML_EXTENSION;
		// std::cout << "extension: " << extension << std::endl;
		// std::cout << "extension: " << extension << std::endl;
    //     if (isCGI(filePath)) {
		// 	extension = HTML_EXTENSION;
		// // std::cout << "extension: " << extension << std::endl;
    //         // content = handleCGI(uri);

		// }
        std::string mimeType = getMimeType(extension);
        eTag = generateETag(filePath, date, lastMfd);

        responseHeaders << httpStatusCode(200) << CRLF
                        << "Date: " << date << CRLF
                        << "Server: " << _serverConfig["server_name"] << CRLF
                        << "Last-Modified: " << lastMfd << CRLF
                        << "ETag: " << eTag << CRLF
                        << "Content-Length: " << content.size() << CRLF
                        << "Cache-Control: no-cache" << CRLF
                        << "Content-Type: " << mimeType << CRLF
                        << "Connection: keep-alive" << CRLF << CRLF;

        if (_requestMap["method"] == "HEAD") {
            return responseHeaders.str();
        }

        responseHeaders << content;
        return responseHeaders.str();
    } else {
        return generateErrorPage(404); // Not Found
    }
}


std::string Get::getRedirection(std::string const &uri) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_redirections.end();
	std::string fixedURI = uri;
	if (fixedURI[fixedURI.size() - 1] == '/')
		fixedURI = fixedURI.erase(fixedURI.size() - 1);

	for (iterator = this->_redirections.begin(); iterator != iteratorEnd; iterator++) {
		if (iterator->first == fixedURI) {

			return iterator->second;
		}
	}

	return "";
}

void Get::setRedirections(void) {
	std::vector<LocationConf>::iterator iterator;
	std::vector<LocationConf>::iterator iteratorEnd = this->_locations.end();

	for (iterator = this->_locations.begin(); iterator != iteratorEnd; iterator++) {
		if ((iterator->getSettings().find("redirect") != iterator->getSettings().end()) && (iterator->getSettings().find("uri") != iterator->getSettings().end())) {
			const std::string uri = iterator->getASettingValue("uri");
			const std::string redirect = iterator->getASettingValue("redirect");
			this->_redirections[uri] = redirect;
		}
	}
}

void Get::printRedirections(void) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_redirections.end();

	std::cout << "Redirections:" << std::endl;

	for (iterator = this->_redirections.begin(); iterator != iteratorEnd; iterator++) {
		std::cout << iterator->first << " = " << iterator->second << std::endl;
	}

}