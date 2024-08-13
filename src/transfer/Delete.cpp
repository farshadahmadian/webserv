/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:06:15 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/13 11:37:06 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(void) : _storageDirectory("../../files"), _responses(std::map<int, std::string>()),  _deleteData(std::map<std::string, std::string>()) {
	return;
}

Delete::Delete(Delete const &other) : _storageDirectory(other._storageDirectory), _responses(other._responses), _deleteData(other._deleteData) {
	return;
}

Delete &Delete::operator=(Delete const &rhs) {
	(void)rhs;
	return *this;
}

Delete::~Delete(void) {
	return;
}

std::string const &Delete::getSocketResponse(int connectedSocketFd) {
	return this->_responses[connectedSocketFd];
}

void Delete::removeSocketResponse(int connectedSocketFd) {
	this->_responses.erase(connectedSocketFd);
}

std::string Delete::getSubStringFromMiddleToIndex(std::string &string, std::string const &toFind, size_t startOffset, size_t endIndex) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(foundIndex + startOffset, endIndex);
	return result;
}

std::string Delete::getSubStringFromStartToIndex(std::string &string, std::string const &toFind) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(0, foundIndex);
	return result;
}

std::string Delete::getFileName(std::string string) {
	std::string toFind = "\r\n\r\n";
	string = getSubStringFromMiddleToIndex(string, toFind, toFind.length(), std::string::npos);

	toFind = "\r\n";
	std::string name = getSubStringFromStartToIndex(string, toFind);
	this->_deleteData["filename"] = name;
	return name;
}

void Delete::getSubmitedData(std::string &contentDisposition) {
	if (!contentDisposition.length())
		return;
	std::string toFind = "name";
	std::string nameAttribute = getSubStringFromMiddleToIndex(contentDisposition, toFind, 0, std::string::npos);
	if (nameAttribute.empty())
		return;
	std::string name = "name=\"";
	std::string nameValue = nameAttribute.substr(name.length(), nameAttribute.find('"') - 1);

	if (nameValue == "name")
		getFileName(nameAttribute);
	return;
}

void Delete::getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter) {
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

std::string Delete::getDelimiter(std::string request) {
	
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

void Delete::parseDeleteRequest(std::string const &requestHeader, std::ostringstream const &requestBody) {
	std::string formFieldsDelimiter = this->getDelimiter(requestHeader);
	std::string body = requestBody.str();
	this->getSubmittedFormInputs(body, formFieldsDelimiter);
}

void Delete::handleDelete(ConnectedSocket &connectedSocket) {
	
	if (connectedSocket.getRequestMap()["uri"] != "/delete") {
		std::string html = "<html><body><h1>Bad Request</h1></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 400 Bad Request\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocket.getSocketFd()] = ostring.str(); 
		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		return;
	}

	this->parseDeleteRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());

	// std::cout << RED << this->_deleteData["filename"] << RESET << std::endl;

	if (this->_deleteData["filename"].empty()) {
		std::string html = "<html><body><h1>Something went wrong</h1></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 400 Bad Request\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocket.getSocketFd()] = ostring.str(); 
		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		return;
}

	std::string message = "File " + this->_deleteData["filename"] + " is deleted.";
	std::string html = "<html><body><h1>" + message + "</h1><a href=\"index.html\">Back to Homepage</a></body></html>";
	std::ostringstream ostring;
	ostring << "HTTP/1.1 200 OK\r\n";
	ostring << "Content-Type: text/html\r\n";
	ostring << "Connection: close\r\n";
	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	ostring << html;
	this->_responses[connectedSocket.getSocketFd()] = ostring.str();
	std::cout << CYAN << "POST RESPONSE:\n" << this->_responses[connectedSocket.getSocketFd()] << RESET << std::endl;
	
}
