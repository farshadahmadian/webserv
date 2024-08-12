/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:29:21 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/12 13:37:40 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(void) : _responses(std::map<int, std::string>()), _postData(std::map<std::string, std::string>()) {
	return;
}

Post::Post(Post const &other) : _responses(other._responses), _postData(other._postData) {
	return;
}

Post &Post::operator=(Post const &rhs) {
	if (this != &rhs)
	{
		this->_responses = rhs._responses;
		this->_postData = rhs._postData;
	}
	return *this;
}

Post::~Post(void) {
	return;
}

std::map<int, std::string> &Post::getResponses(void) {
	return this->_responses;
}

std::map<std::string, std::string> &Post::getPostData(void) {
	return this->_postData;
}

std::string Post::getSubStringFromMiddleToIndex(std::string &string, std::string const &toFind, size_t startOffset, size_t endIndex) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(foundIndex + startOffset, endIndex);
	return result;
}

std::string Post::getSubStringFromStartToIndex(std::string &string, std::string const &toFind) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(0, foundIndex);
	return result;
}

std::string Post::getDelimiter(std::string request) {
	
	std::string toFind = "Content-Type";
	std::string result = getSubStringFromMiddleToIndex(request, toFind, 0, std::string::npos);

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

std::string Post::getBody(std::string request) {
	std::string toFind = "\r\n\r\n";
	std::string body = getSubStringFromMiddleToIndex(request, toFind, toFind.size(), std::string::npos);
	return body;
}

std::string Post::getName(std::string string) {
	// std::cout << "contentDisposition = " << string << std::endl;
	std::string toFind = "\r\n\r\n";
	string = getSubStringFromMiddleToIndex(string, toFind, toFind.length(), std::string::npos);

	toFind = "\r\n";
	std::string name = getSubStringFromStartToIndex(string, toFind);
	// std::cout << "name = " << name << std::endl;
	this->_postData["name"] = name;
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
	this->_postData["filename"] = fileName;
	return fileName;
}

void Post::saveFile(std::string string) {
	std::string fileName = this->_postData["filename"];
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
		if (!(this->_postData["filename"].empty()) && !(this->_postData["name"].empty()))
			saveFile(nameAttribute);
	}
	return;
}

void Post::getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter) {
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

void Post::handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket) {

	// std::cout << "POST REQUEST = \n" << request << std::endl;

	// size_t index = connectedSocket.getRequestHeader().find("Content-Type: multipart/form-data");
	// if (index == std::string::npos) {
	// 	std::ostringstream ostring;
	// 	ostring << "HTTP/1.1 400 Bad Request\r\n";
	// 	ostring << "Connection: close\r\n\r\n";
	// 	this->_responses[connectedSocketFd] = ostring.str(); 
	// 	// this->printPostData();
	// 	std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
	// 	return;
	// }

	if (connectedSocket.getRequestMap()["uri"] != "/submit") {
		
		std::string html = "<html><body><h1>Bad Request</h1></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 400 Bad Request\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocketFd] = ostring.str(); 
		// this->printPostData();
		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		return;
	}

	parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());

	std::cout << "name = " << this->_postData["name"] << "filename = " << this->_postData["filename"] << std::endl;

	if (this->_postData["name"].empty() || this->_postData["filename"].empty()) {
		std::string html = "<html><body><h1>Something went wrong</h1></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 400 Bad Request\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocketFd] = ostring.str(); 
		// this->printPostData();
		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		return;
}

	std::string message = "Thank you " + this->_postData["name"] + ", file " + this->_postData["filename"] + " is Received.";
	std::string html = "<html><body><h1>" + message + "</h1><a href=\"index.html\">Back to Homepage</a></body></html>";
	std::ostringstream ostring;
	ostring << "HTTP/1.1 200 OK\r\n";
	ostring << "Content-Type: text/html\r\n";
	ostring << "Connection: close\r\n";
	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	ostring << html;
	this->_responses[connectedSocketFd] = ostring.str();
	std::cout << CYAN << "POST RESPONSE:\n" << this->_responses[connectedSocketFd] << RESET << std::endl;
	this->printPostData();
	this->printPostResponses();
	return;
}

void Post::printPostData(void) {
	if (!_postData.size())
	{
		std::cout << YELLOW << "POST DATA IS EMPTY" << RESET << std::endl;
		return;
	}

	std::map<std::string, std::string>::iterator iterator = this->_postData.begin();

	std::cout << YELLOW << "DATA RECEIVED FROM FORM SUBMISSION:" << RESET <<std::endl;

	while (iterator != this->_postData.end())
	{
		std::cout << YELLOW << iterator->first << ": " << iterator->second << RESET << std::endl;
		iterator++;
	}
}

void Post::printPostResponses(void) {
	if (!this->_responses.size())
	{
		std::cout << BLUE << "POST RESPONSES MAP IS EMPTY" << RESET << std::endl;
		return;
	}
	std::map<int, std::string>::iterator iterator;
	std::map<int, std::string>::iterator iteratorEnd = this->_responses.end();

	for (iterator = this->_responses.begin(); iterator != iteratorEnd; iterator++)
	std::cout << BLUE << "Connected socket [" << iterator->first << "] sends the respons:\n" << iterator->second << RESET << std::endl;
	return;
}
