/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:29:21 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/27 17:50:40 by fahmadia         ###   ########.fr       */
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

std::string Post::getDelimiter(std::string request) {
	
	std::string toFind = "Content-Type";
	size_t foundIndex = request.find(toFind);
	std::string result = request.substr(foundIndex, std::string::npos);

	toFind = "boundary=";
	size_t sizeOfToFind = toFind.size();
	foundIndex = result.find(toFind);


	toFind = "\r\n";
	size_t temp = result.find(toFind);
	std::string formFieldsDelimiter = result.substr(foundIndex + sizeOfToFind, temp - foundIndex - sizeOfToFind);
	
	formFieldsDelimiter = "--" + formFieldsDelimiter;
	return formFieldsDelimiter;
}

std::string Post::getBody(std::string request) {
	std::string toFind = "\r\n\r\n";
	size_t bodyStartIndex = request.find(toFind);
	std::string body = request.substr(bodyStartIndex + toFind.length(), std::string::npos);

	return body;
}

std::string Post::getName(std::string string) {
	// std::cout << "contentDisposition = " << string << std::endl;
	
	// std::string toFind = "\r\n\r\n";
	// std::string name =  string.substr(string.find(toFind) + toFind.length(), string.find_last_of("\r\n") - 2);
	// std::cout << "name = " << name << std::endl;
	std::string toFind = "\r\n\r\n";
	size_t nameStartIndex = string.find(toFind);
	string = string.substr(nameStartIndex + toFind.size(), std::string::npos);
	std::string name = string.substr(0, string.find("\r\n"));
	// std::cout << "name = " << name << std::endl;
	this->_postData["name"] = name;
	return name;
}

std::string Post::getFileName(std::string string) {
	// std::cout << "contentDisposition = " << string << std::endl;
	std::string toFind = "filename=\"";
	std::string fileNameAttribute = string.substr(string.find(toFind), std::string::npos);

	std::string fileName = fileNameAttribute.substr(toFind.length(), fileNameAttribute.find_last_of('"') - toFind.length());
	this->_postData["filename"] = fileName;
	return fileName;
}

void Post::saveFile(std::string string) {
	std::string fileName = this->_postData["name"] + "_file";
	std::ofstream outputFileStream(fileName);

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

	std::string toFind = "name";
	std::string nameAttribute = contentDisposition.substr(contentDisposition.find(toFind), std::string::npos);

	std::string name = "name=\"";
	std::string nameValue = nameAttribute.substr(name.length(), nameAttribute.find('"') - 1);
	// std::cout << "nameValue = " << nameValue << std::endl;

	if (nameValue == "name")
		getName(nameAttribute);
	if (nameValue == "file")
	{
		getFileName(nameAttribute);
		saveFile(nameAttribute);
	}
	return;
}

void Post::getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter) {
	while (true)
	{
		size_t contentDispositionIndex = body.find("Content-Disposition");

		if (contentDispositionIndex == std::string::npos)
			break;

		body = body.substr(contentDispositionIndex, std::string::npos);

		size_t firstDelimiterIndex = body.find(formFieldsDelimiter);

		if (firstDelimiterIndex == std::string::npos)
			break;

		std::string contentDisposition = body.substr(0, firstDelimiterIndex);
		
		getSubmitedData(contentDisposition);
		body = body.substr(firstDelimiterIndex, std::string::npos);
	}
	
	return;
}

void Post::parsePostRequest(std::string request) {

	std::string formFieldsDelimiter = getDelimiter(request);
	std::string body = getBody(request);
	getSubmittedFormInputs(body, formFieldsDelimiter);
}

void Post::handlePost(std::string request, int connectedSocketFd) {
	parsePostRequest(request);
	std::ostringstream ostring;
	ostring << "HTTP/1.1 201 Created\r\n";
	this->_responses[connectedSocketFd] = ostring.str(); 
	this->printPostData();
	return;
}

void Post::printPostData(void) {
	std::map<std::string, std::string>::iterator iterator = this->_postData.begin();

	std::cout << YELLOW << "DATA RECEIVED FROM FORM SUBMISSION:" << RESET <<std::endl;

	while (iterator != this->_postData.end())
	{
		std::cout << YELLOW << iterator->first << ": " << iterator->second << RESET << std::endl;
		iterator++;
	}
	
}