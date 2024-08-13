/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:06:15 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/13 18:44:20 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(void) : _storageDirectory("files"), _responses(std::map<int, std::string>()),  _deleteData(std::map<std::string, std::string>()) {
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

void Delete::clearDeleteData(void) {
	this->_deleteData.clear();
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

	if (formFieldsDelimiter.empty()) {
		std::string toFind = "name=";
		std::string fileName = getSubStringFromMiddleToIndex(body, toFind, toFind.size(), std::string::npos);
		this->_deleteData["filename"] = fileName;
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
	std::cout << YELLOW << "finding the file and deleting it . . ." << RESET << std::endl;

	DIR *openDirectory = opendir(this->_storageDirectory.c_str());
	if (!openDirectory) {
		std::string html = "<html><body><h1>Bad Request, Storage Directory (/file) Does Not Exist </h1></body></html>";
		std::ostringstream ostring;
		ostring << "HTTP/1.1 400 Bad Request\r\n";
		ostring << "Content-Type: text/html\r\n";
		ostring << "Connection: close\r\n";
		ostring << "Content-Length: " << html.length() << "\r\n\r\n";
		ostring << html;
		this->_responses[connectedSocket.getSocketFd()] = ostring.str(); 
		std::cout << RED << "RESPONSE:\n" << ostring.str() << RESET << std::endl;
		return false;
	}
	std::cout << openDirectory << std::endl;

	std::string fileToDelete = this->_storageDirectory + "/" + this->_deleteData["filename"];
	std::cout << "file to be deleted: " << fileToDelete << std::endl;
	struct stat buffer;
	memset(&buffer, 0, sizeof(buffer));
	stat(fileToDelete.c_str(), &buffer);
	std::cout << buffer.st_size << std::endl;
	// int result = access(fileToDelete.c_str(), F_OK);
	// if (result == 0)
	// 	std::cout << "file: " << fileToDelete << " exists" << std::endl;
	// else
	// 	std::cout << "file: " << fileToDelete << " does not exist" << std::endl;

	// if (result == 0) {
	// 	if ((result = access(fileToDelete.c_str(), W_OK)) == 0) 
	// 		std::cout << "file: " << fileToDelete << " has write access" << std::endl;
	// 	else
	// 		std::cout << "file: " << fileToDelete << " does not have write access" << std::endl;
	// }



	int isWritable = 0;
	int exist = 0;
	
	if ((isWritable = access(fileToDelete.c_str(), W_OK)) == 0)
	{
		std::cout << fileToDelete << " has write permission " << std::endl;
	}
	else if ((exist = access(fileToDelete.c_str(), F_OK)) == 0)
		std::cout << "file " << fileToDelete << " cannot be deleted due to permissions" << std::endl;
	else
		std::cout << "file " << fileToDelete << " does not exist" << std::endl;
	
	if (isWritable == 0) {
		DIR *test = opendir(fileToDelete.c_str());
		if (test)
			std::cout << fileToDelete << " is a directory, and not a file!" << std::endl;
		else
		{
			std::cout << fileToDelete << " is a file, and not a directory." << std::endl;
			remove(fileToDelete.c_str());
			std::cout << "File " << fileToDelete << " is deleted" << std::endl;
		}
	}

	return true;
}

void Delete::handleDelete(ConnectedSocket &connectedSocket) {
	
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

	if (!this->deleteFile(connectedSocket))
		return;

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
	return;
}
