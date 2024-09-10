/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:29:21 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/10 09:32:11 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"
#include "Server.hpp"

Post::Post(void) : HTTPResponse(), _isFileSaved(true) {
	this->_cgiDirectory = "/cgi-post/";
	return;
}

Post::Post(Post const &other) : HTTPResponse(other) {
	this->_cgiDirectory = other._cgiDirectory;
	return;
}

Post &Post::operator=(Post const &rhs) {
	if (this != &rhs) {
		this->_isFileSaved = rhs._isFileSaved;
		this->_cgiDirectory = rhs._cgiDirectory;
	}
	return *this;
}

Post::~Post(void) {
	return;
}

std::string Post::getDelimiter(std::string request) {
	
	std::string toFind = "Content-Type";
	std::string result = getSubStringFromMiddleToIndex(request, toFind, 0, std::string::npos);

	if (result.empty())
		return "";

	toFind = "boundary=";
	size_t sizeOfToFind = toFind.size();
	result = getSubStringFromMiddleToIndex(result, toFind, sizeOfToFind, std::string::npos);

	toFind = "\r\n";
	result = getSubStringFromStartToIndex(result, toFind);
	if (result.empty())
		return "";
	std::string formFieldsDelimiter = "--" + result;
	return formFieldsDelimiter;
}

std::string Post::getName(std::string string) {
	std::string toFind = "\r\n\r\n";
	string = getSubStringFromMiddleToIndex(string, toFind, toFind.length(), std::string::npos);

	toFind = "\r\n";
	std::string name = getSubStringFromStartToIndex(string, toFind);
	this->_data["name"] = name;
	return name;
}

std::string Post::getFileName(std::string string) {
	std::string toFind = "filename=\"";
	std::string fileNameAttribute = getSubStringFromMiddleToIndex(string, toFind, 0, std::string::npos);

	if (fileNameAttribute.empty())
		return "";

	std::string removeFromLast = "\"";
	std::string lineFeed = "\r\n";
	size_t lineFeedIndex = fileNameAttribute.find("\r\n");
	std::string fileName = "";
	if (lineFeedIndex != std::string::npos)
		fileName = fileNameAttribute.substr(toFind.length(), fileNameAttribute.find("\r\n") - toFind.length() - removeFromLast.length());
	this->_data["filename"] = fileName;
	return fileName;
}

void Post::saveFile(std::string string) {
	DIR *directory;
	if (!(directory = opendir(this->getStorageDirectory().c_str()))) {
		this->_isFileSaved = false;
		return;
	}
	closedir(directory);

	std::string fileName = this->getStorageDirectory() + "/" + this->_data["filename"];
	std::ofstream outputFileStream(fileName.c_str(), std::ios::binary);

	std::string toFind = "\r\n\r\n";
	std::string temp = string.substr(string.find(toFind) + toFind.length(), std::string::npos);
	std::string fileContent =  temp.substr(0, temp.find_last_of("\r\n") - 1);
	outputFileStream << fileContent;
	outputFileStream.close();
	return;
}

void Post::getSubmitedData(std::string &contentDisposition) {
	if (!contentDisposition.length())
		return;
	std::string toFind = "name";
	std::string nameAttribute = getSubStringFromMiddleToIndex(contentDisposition, toFind, 0, std::string::npos);

	if (nameAttribute.empty())
		return;

	std::string name = "name=\"";
	std::string nameValue = nameAttribute.substr(name.length(), nameAttribute.find('"') - 1);

	if (nameValue == "name")
		getName(nameAttribute);
	if (nameValue == "file")
	{
		getFileName(nameAttribute);
		if (!(this->_data["filename"].empty()) && !(this->_data["name"].empty()))
			saveFile(nameAttribute);
	}
	return;
}

void Post::getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter) {

	if (formFieldsDelimiter.empty())
		return;

	while (body.size())
	{
		std::string toFind = "Content-Disposition";
		body = getSubStringFromMiddleToIndex(body, toFind, 0, std::string::npos);
		std::string contentDisposition = getSubStringFromStartToIndex(body, formFieldsDelimiter);
		getSubmitedData(contentDisposition);
		body = getSubStringFromMiddleToIndex(body, formFieldsDelimiter, 0, std::string::npos);
	}
	
	return;
}

void Post::parsePostRequest(std::string const &requestHeader, std::ostringstream const &requestBody) {

	std::string formFieldsDelimiter = this->getDelimiter(requestHeader);
	std::string body = requestBody.str();
	this->getSubmittedFormInputs(body, formFieldsDelimiter);
}

std::string Post::handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket, std::map<std::string, std::string> &serverConfig) {

	size_t maxBodySize = static_cast<size_t>(Server::stringToInt(serverConfig["client_max_body_size"]));

	if (connectedSocket.getRequestBody().str().length() > maxBodySize) {
		this->_responses[connectedSocketFd] = generateErrorPage(413);
		return(this->_responses[connectedSocketFd]);
	} 

	if (isCgiUri(connectedSocket)) {
		std::string response = this->handleCgi(connectedSocket);
		return response;
	}

	if (connectedSocket.getRequestMap()["Content-Type"] == "plain/text") {

		std::string html = "<html><body><h1>" + connectedSocket.getRequestBody().str() + "</h1><a href=\"index.html\">Back to Homepage</a></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 200 OK\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocketFd] = ostring.str();
		return (this->_responses[connectedSocketFd]);
	}

	if (connectedSocket.getRequestMap()["uri"] != "/submit") {
		this->_responses[connectedSocketFd] = generateErrorPage(400);
		return (this->_responses[connectedSocketFd]);
	}
	parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());
	if (this->_data["name"].empty() || this->_data["filename"].empty()) {
		this->_responses[connectedSocketFd] = generateErrorPage(400);
		return (this->_responses[connectedSocketFd]);
	}
	if (!this->_isFileSaved)
	{
		this->_responses[connectedSocketFd] = generateErrorPage(500);
		return (this->_responses[connectedSocketFd]);
	}
	std::string message = "Thank you " + this->_data["name"] + ", file " + this->_data["filename"] + " is Received, and Stored in" + this->_storageDirectory +".";
	std::string html = "<html><body><h1>" + message + "</h1><a href=\"form/index.html\">Back</a></body></html>";
	std::ostringstream ostring;
	ostring << "HTTP/1.1 200 OK\r\n";
	ostring << "Content-Type: text/html\r\n";
	ostring << "Connection: close\r\n";
	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	ostring << html;
	this->_responses[connectedSocketFd] = ostring.str();
	return (this->_responses[connectedSocketFd]);
}
