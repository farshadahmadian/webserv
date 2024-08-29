/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:41:26 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/28 19:21:49 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"

GetHandler::GetHandler(const std::map<std::string, std::string>& requestMap,
					   const std::map<std::string, std::string>& serverConfig, std::vector<LocationConf> const &locations) :
	HTTPResponse(serverConfig, locations) {
	setRequestMapInResponse(requestMap);
	this->setRedirections();
	// this->printRedirections();
}

GetHandler::~GetHandler() {}

/*Generate the directory list inside the directory that is passed to it.*/
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

/*Return the correct address of the index file inside the received directory address
in case if its existence.*/
std::string GetHandler::findIndexFile(const std::string& dirPath) {
    std::vector<std::string> indexFiles;
    indexFiles.push_back("index.html");
    indexFiles.push_back("index.htm");
    indexFiles.push_back("default.html");
    struct stat st;

    for (std::vector<std::string>::iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
        const std::string& indexFile = *it;
        std::string fullPath = dirPath + "/" + indexFile;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
            Server::logMessage("INFO: Index File Found: " + fullPath);
            return fullPath;
        }
    }

    return ""; // Return empty string if no index file is found
}

std::string const GetHandler::setServerRoot(std::string const & filePath) {
    if (filePath == "./www//farshad/")
        return ("localhost:8080");
    if (filePath == "./www//navid/")
        return ("localhost:8081");
    if (filePath == "./www//nico/")
        return ("localhost:8082");
    return ("localhost:8080");
}

/*Check whether the passed file path is directory or not.*/
bool isFile(std::string const & filePath) {
    struct stat st;
	if (stat(filePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
		return (false);
	return (true);
}


std::string GetHandler::GetMethod() {
	std::string content;
	std::string date, lastMfd, eTag;
	std::ostringstream responseHeaders;
	std::string uri = _requestMap["uri"];
	std::string filePath = _serverConfig.at("root") + uri;

	if (getRedirections(uri).length()) {
		std::cout << GREEN << "Redirecting from " << uri << " to " << getRedirections(uri) << RESET << std::endl;
		responseHeaders << httpStatusCode(302) << CRLF
										<< "Location: " << getRedirections(uri) << CRLF
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
    if (!content.empty() || isCGI(filePath)) {
        std::string extension = filePath.substr(filePath.find_last_of("."));
		// std::cout << "extension: " << extension << std::endl;
        if (isCGI(filePath)) {
			extension = HTML_EXTENSION;
		// std::cout << "extension: " << extension << std::endl;
            content = handleCGI(uri);

		}
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


std::string GetHandler::getRedirections(std::string const &uri) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_redirections.end();

	for (iterator = this->_redirections.begin(); iterator != iteratorEnd; iterator++) {
		if (iterator->first == uri)
			return iterator->second;
	}

	return "";
}

void GetHandler::setRedirections(void) {
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

void GetHandler::printRedirections(void) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_redirections.end();

	std::cout << "Redirections:" << std::endl;

	for (iterator = this->_redirections.begin(); iterator != iteratorEnd; iterator++) {
		std::cout << iterator->first << " = " << iterator->second << std::endl;
	}

}