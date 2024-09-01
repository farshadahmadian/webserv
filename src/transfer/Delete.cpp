/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:06:15 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/01 18:55:33 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"
#include "Server.hpp"

Delete::Delete(void) : HTTPResponse() {
	return;
}

Delete::Delete(Delete const &other) : HTTPResponse(other) {
	return;
}

Delete &Delete::operator=(Delete const &rhs) {
	(void)rhs;
	return *this;
}

Delete::~Delete(void) {
	return;
}

std::string Delete::getFileName(std::string string) {
	std::string toFind = "\r\n\r\n";
	string = getSubStringFromMiddleToIndex(string, toFind, toFind.length(), std::string::npos);

	toFind = "\r\n";
	std::string name = getSubStringFromStartToIndex(string, toFind);
	this->_data["filename"] = name;
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

	if (formFieldsDelimiter.empty()) {
		std::string toFind = "name=";
		std::string fileName = getSubStringFromMiddleToIndex(body, toFind, toFind.size(), std::string::npos);
		this->_data["filename"] = fileName;
		std::cout << RED << "*** file name = " << fileName <<  RESET << std::endl;
		return;
	}

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

	if (result.find("Content-Type: text/plain;") != std::string::npos) {
		toFind = "boundary: ";
		size_t sizeOfToFind = toFind.size();
		result = getSubStringFromMiddleToIndex(request, toFind, sizeOfToFind, std::string::npos);
	}
	else if (result.find("Content-Type: multipart/form-data;") != std::string::npos) {
		toFind = "boundary=";
		size_t sizeOfToFind = toFind.size();
		result = getSubStringFromMiddleToIndex(request, toFind, sizeOfToFind, std::string::npos);
	}
	else
		return "";

	toFind = "\r\n";
	result = getSubStringFromStartToIndex(result, toFind);
	if (result.empty())
		return "";
	std::string formFieldsDelimiter = "--" + result;
	return formFieldsDelimiter;
}

void Delete::parseDeleteRequest(std::string const &requestHeader, std::ostringstream const &requestBody) {
	std::string formFieldsDelimiter = this->getDelimiter(requestHeader);
	// if (formFieldsDelimiter.empty())
	// 	return;
	std::string body = requestBody.str();
	this->getSubmittedFormInputs(body, formFieldsDelimiter);
}

bool Delete::deleteFile(ConnectedSocket &connectedSocket) {
	std::cout << YELLOW << "Deleting ..." << RESET << std::endl;

	DIR *directory = opendir(this->getStorageDirectory().c_str());
	if (!directory) {
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(500);
		return false;
	}
	// std::cout << directory << std::endl;
	closedir(directory);
	std::string fileToDelete = this->getStorageDirectory() + "/" + this->_data["filename"];
	std::cout << YELLOW << "To delete: " << fileToDelete << RESET << std::endl;

	int isWritable = 0;
	int exist = 0;
	
	if ((exist = access(fileToDelete.c_str(), F_OK)) == 0)
	{
		std::cout << YELLOW << fileToDelete << " exists. " << YELLOW << std::endl;
	}
	else
	{
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(404); 
		return (false);
	}
	
	if ((isWritable = access(fileToDelete.c_str(), W_OK)) == 0)
	{
		std::cout << YELLOW << fileToDelete << " is writable." << RESET << std::endl;

		directory = opendir(fileToDelete.c_str());
		if (directory)
		{
			Server::logMessage("INFO: " + fileToDelete + " is a directory, and not a file!");
			std::cout << YELLOW << fileToDelete << " is a directory, and not a file!" << RESET << std::endl;
			closedir(directory);
			this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(403); 
			return false;
		}
		else
		{
			std::cout << YELLOW << fileToDelete << " is a file, and not a directory." << RESET << std::endl;
			std::remove(fileToDelete.c_str());
			std::cout << YELLOW << fileToDelete << " is deleted" << RESET << std::endl;
			return true;
		}
	}
	else {
		std::cout << YELLOW << fileToDelete << " is not writable" << RESET << std::endl;
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(403); 
		return false;
	}
	
	

	

	return true;
}

std::string const & Delete::handleDelete(ConnectedSocket &connectedSocket) {
	
	// if (connectedSocket.getRequestMap()["uri"] != "/delete") {
	// 	std::string html = "<html><body><h1>Bad Request</h1></body></html>";
	// 	std::ostringstream ostring;
	// 	ostring << "HTTP/1.1 400 Bad Request\r\n";
	// 	ostring << "Content-Type: text/html\r\n";
	// 	ostring << "Connection: close\r\n";
	// 	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	// 	ostring << html;
	// 	this->_responses[connectedSocket.getSocketFd()] = ostring.str(); 
	// 	std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
	// 	return;
	// }

	this->parseDeleteRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());

	// std::cout << RED << this->_data["filename"] << RESET << std::endl;

	if (this->_data["filename"].empty()) {
		// std::string html = "<html><body><h1>Something went wrong</h1></body></html>";
		// std::ostringstream ostring;
		// ostring << "HTTP/1.1 400 Bad Request\r\n";
		// ostring << "Content-Type: text/html\r\n";
		// ostring << "Connection: close\r\n";
		// ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		// ostring << html;
		// this->_responses[connectedSocket.getSocketFd()] = ostring.str(); 
		// std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(400); 
		return (this->_responses[connectedSocket.getSocketFd()]);
}

	if (!this->deleteFile(connectedSocket))
		return (this->_responses[connectedSocket.getSocketFd()]);

	std::string message = "File " + this->_data["filename"] + " is deleted.";
	std::string html = "<html><body><h1>" + message + "</h1><a href=\"form/index.html\">Back</a></body></html>";
	std::ostringstream ostring;
	ostring << "HTTP/1.1 200 OK\r\n";
	ostring << "Content-Type: text/html\r\n";
	ostring << "Connection: close\r\n";
	ostring << "Content-Length: " << html.length() << "\r\n\r\n";
	ostring << html;
	this->_responses[connectedSocket.getSocketFd()] = ostring.str();
	// std::cout << CYAN << "DELETE RESPONSE:\n" << this->_responses[connectedSocket.getSocketFd()] << RESET << std::endl;
	return (this->_responses[connectedSocket.getSocketFd()]);
}
