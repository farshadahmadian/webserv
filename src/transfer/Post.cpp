/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:29:21 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/27 15:23:57 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(void) : HTTPResponse(), _isFileSaved(true) {
	return;
}

Post::Post(Post const &other) : HTTPResponse(other) {
	return;
}

Post &Post::operator=(Post const &rhs) {
	if (this != &rhs)
		this->_isFileSaved = rhs._isFileSaved;
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

// std::string Post::getBody(std::string request) {
// 	std::string toFind = "\r\n\r\n";
// 	std::string body = getSubStringFromMiddleToIndex(request, toFind, toFind.size(), std::string::npos);
// 	return body;
// }

std::string Post::getName(std::string string) {
	// std::cout << "contentDisposition = " << string << std::endl;
	std::string toFind = "\r\n\r\n";
	string = getSubStringFromMiddleToIndex(string, toFind, toFind.length(), std::string::npos);

	toFind = "\r\n";
	std::string name = getSubStringFromStartToIndex(string, toFind);
	// std::cout << "name = " << name << std::endl;
	this->_data["name"] = name;
	return name;
}

std::string Post::getFileName(std::string string) {
	// std::cout << "contentDisposition = " << string << std::endl;
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
	// std::string storageDirectory = "files/";
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
	// std::cout << "temp = " << temp << std::endl;
	// size_t contentStartIndex = string.find(toFind) + toFind.length(); 
	// std::string temp = string.substr(contentStartIndex, std::string::npos);
	// std::string fileContent = temp.substr(0, temp.find("\r\n"));
	// std::cout << temp.find_last_of("\r\n") << std::endl;
	std::string fileContent =  temp.substr(0, temp.find_last_of("\r\n") - 1);
	outputFileStream << fileContent;
	outputFileStream.close();
	// std::cout << "file Content = = =  " << outputFileStream. << std::endl;
	return;
}

void Post::getSubmitedData(std::string &contentDisposition) {
	// std::cout << "contentDisposition = " << contentDisposition << std::endl;
	if (!contentDisposition.length())
		return;
	std::string toFind = "name";
	// std::string nameAttribute = contentDisposition.substr(contentDisposition.find(toFind), std::string::npos);
	std::string nameAttribute = getSubStringFromMiddleToIndex(contentDisposition, toFind, 0, std::string::npos);

	if (nameAttribute.empty())
		return;

	std::string name = "name=\"";
	std::string nameValue = nameAttribute.substr(name.length(), nameAttribute.find('"') - 1);
	// std::cout << "nameValue = " << nameValue << std::endl;

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
		// size_t contentDispositionIndex = body.find("Content-Disposition");

		// if (contentDispositionIndex == std::string::npos)
		// 	break;

		// body = body.substr(contentDispositionIndex, std::string::npos);
		body = getSubStringFromMiddleToIndex(body, toFind, 0, std::string::npos);
		// size_t delimiterIndex = body.find(formFieldsDelimiter);

		// if (delimiterIndex == std::string::npos)
		// 	break;

		// std::string contentDisposition = body.substr(0, delimiterIndex);

		std::string contentDisposition = getSubStringFromStartToIndex(body, formFieldsDelimiter);

		getSubmitedData(contentDisposition);
		// body = body.substr(delimiterIndex, std::string::npos);
		body = getSubStringFromMiddleToIndex(body, formFieldsDelimiter, 0, std::string::npos);
	}
	
	return;
}

void Post::parsePostRequest(std::string const &requestHeader, std::ostringstream const &requestBody) {

	std::string formFieldsDelimiter = this->getDelimiter(requestHeader);
	std::string body = requestBody.str();
	// std::cout << "body:\n" << body << std::endl;
	this->getSubmittedFormInputs(body, formFieldsDelimiter);
}

std::string const & Post::handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket) {
	if (connectedSocket.getRequestMap()["uri"] != "/submit") {
		this->_responses[connectedSocketFd] = generateErrorPage(400);
		return (this->_responses[connectedSocketFd]);
	}
	parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());
	std::cout << "name = " << this->_data["name"] << ", filename = " << this->_data["filename"] << std::endl;
	if (this->_data["name"].empty() || this->_data["filename"].empty()) {
		this->_responses[connectedSocketFd] = generateErrorPage(400);
		return (this->_responses[connectedSocketFd]);
	}
	if (!this->_isFileSaved)
	{
		this->_responses[connectedSocketFd] = generateErrorPage(500);
		return (this->_responses[connectedSocketFd]);
	}
	std::string message = "Thank you " + this->_data["name"] + ", file " + this->_data["filename"] + " is Received, and Stored in ./files/.";
	std::string html = "<html><body><h1>" + message + "</h1><a href=\"index.html\">Back to Homepage</a></body></html>";
	std::ostringstream ostring;
	ostring << "HTTP/1.1 200 OK\r\n";
	ostring << "Content-Type: text/html\r\n";
	ostring << "Connection: close\r\n";
	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	ostring << html;
	this->_responses[connectedSocketFd] = ostring.str();
	return (this->_responses[connectedSocketFd]);
}
// void Post::handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket) {

// 	// std::cout << "POST REQUEST = \n" << request << std::endl;

// 	// size_t index = connectedSocket.getRequestHeader().find("Content-Type: multipart/form-data");
// 	// if (index == std::string::npos) {
// 	// 	std::ostringstream ostring;
// 	// 	ostring << "HTTP/1.1 400 Bad Request\r\n";
// 	// 	ostring << "Connection: close\r\n\r\n";
// 	// 	this->_responses[connectedSocketFd] = ostring.str(); 
// 	// 	// this->printPostData();
// 	// 	std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
// 	// 	return;
// 	// }

// 	if (connectedSocket.getRequestMap()["uri"] != "/submit") {
		
// 		std::string html = "<html><body><h1>Bad Request</h1></body></html>";
// 		std::ostringstream ostring;
// 		ostring << "HTTP/1.1 400 Bad Request\r\n";
// 		ostring << "Content-Type: text/html\r\n";
// 		ostring << "Connection: close\r\n";
// 		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
// 		ostring << html;
// 		this->_responses[connectedSocketFd] = ostring.str(); 
// 		// this->printPostData();
// 		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
// 		return;
// 	}

// 	parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());

// 	std::cout << "name = " << this->_data["name"] << ", filename = " << this->_data["filename"] << std::endl;

// 	if (this->_data["name"].empty() || this->_data["filename"].empty()) {
// 		std::string html = "<html><body><h1>Something went wrong</h1></body></html>";
// 		std::ostringstream ostring;
// 		ostring << "HTTP/1.1 400 Bad Request\r\n";
// 		ostring << "Content-Type: text/html\r\n";
// 		ostring << "Connection: close\r\n";
// 		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
// 		ostring << html;
// 		this->_responses[connectedSocketFd] = ostring.str(); 
// 		// this->printPostData();
// 		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
// 		return;
// 	}

// 	if (!this->_isFileSaved)
// 	{
// 		std::string html = "<html><body><h1>500 Internal Server Error</h1></body></html>";
// 		std::ostringstream ostring;
// 		ostring << "HTTP/1.1 500 Internal Server Error\r\n";
// 		ostring << "Content-Type: text/html\r\n";
// 		ostring << "Connection: close\r\n";
// 		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
// 		ostring << html;
// 		this->_responses[connectedSocketFd] = ostring.str(); 
// 		// this->printPostData();
// 		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
// 		return;
// 	}

// 	std::string message = "Thank you " + this->_data["name"] + ", file " + this->_data["filename"] + " is Received.";
// 	std::string html = "<html><body><h1>" + message + "</h1><a href=\"index.html\">Back to Homepage</a></body></html>";
// 	std::ostringstream ostring;
// 	ostring << "HTTP/1.1 200 OK\r\n";
// 	ostring << "Content-Type: text/html\r\n";
// 	ostring << "Connection: close\r\n";
// 	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
// 	ostring << html;
// 	this->_responses[connectedSocketFd] = ostring.str();
// 	// std::cout << CYAN << "POST RESPONSE:\n" << this->_responses[connectedSocketFd] << RESET << std::endl;
// 	// this->printData();
// 	// this->printResponses();
// 	return;
// }



