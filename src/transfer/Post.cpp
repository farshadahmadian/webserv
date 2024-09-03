/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:29:21 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/03 08:33:12 by fahmadia         ###   ########.fr       */
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

std::string Post::handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket, std::map<std::string, std::string> &serverConfig) {

	size_t maxBodySize = static_cast<size_t>(Server::stringToInt(serverConfig["client_max_body_size"]));

	if (connectedSocket.getRequestBody().str().length() > maxBodySize) {
		this->_responses[connectedSocketFd] = generateErrorPage(413);
		// std::cout << "******************** BODY IS TOO BIG ********************" << connectedSocket.getRequestBody().str().length() << std::endl;
		return(this->_responses[connectedSocketFd]);
	} else {
		// std::cout << "******************** BODY IS NOT TOO BIG ********************" << connectedSocket.getRequestBody().str().length() << std::endl;
	}

	if (isCgiPostUri(connectedSocket)) {
		std::string response = this->handlePostCgi(connectedSocket);
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




std::string Post::findCommand(std::string const &command) {
	(void)command;
	return "";
}


ChildProcessData Post::createPipeAndFork(ConnectedSocket &connectedSocket) {
	int pipeFds[2];
	// int stdInCopy = dup(STDIN_FILENO);
	// int stdOutCopy = dup(STDOUT_FILENO);

	if (pipe(pipeFds) == -1) {
		cgiError(connectedSocket);
		return connectedSocket._childProcessData ;
	}

	Server::logMessage("pipeFds[0] = " + Server::intToString(pipeFds[0]));
	Server::logMessage("pipeFds[1] = " + Server::intToString(pipeFds[1]));

	pid_t id = fork();
	if (id == -1) {
		close(pipeFds[0]);
		close(pipeFds[1]);
		this->cgiError(connectedSocket);
		return connectedSocket._childProcessData;
	}

	Poll::cgiChildProcessNum++;
	std::cout << "Poll::cgiChildProcessNum = " << Poll::cgiChildProcessNum << std::endl;

	if (id == 0) {		
		this->handleCgiChildProcess(connectedSocket, pipeFds);
		return connectedSocket._childProcessData;
	} else {
		handleCgiMainProcess(connectedSocket, pipeFds, id);
	 	return connectedSocket._childProcessData;;
	}
	return connectedSocket._childProcessData;;
}

void Post::cgiError(ConnectedSocket &connectedSocket) {
	this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(500);
	int fds[2] = {-1, -1};
	this->UpdateCgiProperties(connectedSocket, -1, fds, true);
	connectedSocket.setState(DONE);
	return;
}

void Post::handleCgiChildProcess(ConnectedSocket &connectedSocket, int pipeFds[2]) {
	if (close(pipeFds[0]) == -1) {
			Server::logMessage(Server::intToString(pipeFds[0]) + ": error when closing");
	}
	else {
		Server::logMessage(Server::intToString(pipeFds[0]) + "is closed in child process");

	}
	pipeFds[0] = -1;
	dup2(pipeFds[1], STDOUT_FILENO);
	if (close(pipeFds[1]) == -1) {
		Server::logMessage(Server::intToString(pipeFds[1]) + ": error when closing");
	}
	else {
		Server::logMessage(Server::intToString(pipeFds[1]) + "is closed in child process");
	}
	pipeFds[1] = -1;
	// dup2(STDOUT_FILENO, stdOutCopy);
	// dup2(STDIN_FILENO, stdInCopy);
	this->parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());
	std::string requestData = this->_data["name"];
	std::string keyValue = "NAME=" + requestData;
	// std::cerr << "keyValue = " << keyValue << std::endl;
	char *name = const_cast<char *>(keyValue.c_str());
	char *const env[] = {name, NULL};

	// std::string command = this->findCommand("node");

	// std::string command = "/Users/fahmadia/.nvm/versions/node/v20.15.0/bin/node";
	std::string command = "/usr/bin/python";
	char *cmd = const_cast<char *>(command.c_str());

	// std::string file = "./www/farshad/form/cgi.js";
	std::string file = this->_cgiFilePath;
	char *filePath = const_cast<char *>(file.c_str());

	char *const argv[] = {cmd, filePath, NULL};



	int result = execve(cmd, argv, env);
	std::cerr << RED << "cmd or argv are wrong => execve failed. execve() returned: " << result << RESET << std::endl;
	connectedSocket._isCgiChildProcessReturning = true;
	return;

}

void Post::handleCgiMainProcess(ConnectedSocket &connectedSocket, int pipeFds[2], pid_t id) {
	// dup2(pipeFds[0], STDIN_FILENO);
	this->UpdateCgiProperties(connectedSocket, id, pipeFds, false);

	if (connectedSocket._childProcessData.pipeFds[1] != -1)
	{
		if (close(pipeFds[1]) == -1) {
		connectedSocket._childProcessData.pipeFds[1] = -1;
		Server::logMessage(Server::intToString(pipeFds[1]) + ": error when closing");
		}
		else {
		Server::logMessage(Server::intToString(pipeFds[1]) + "is closed in parent process");
		}
	}
			// close(pipeFds[1]);
		// dup2(STDOUT_FILENO, stdOutCopy);
		// dup2(STDIN_FILENO, stdInCopy);
		// close(connectedSocket._childProcessData.pipeFds[1]);
}

void Post::UpdateCgiProperties(ConnectedSocket &connectedSocket, pid_t id, int pipeFds[2], bool isError) {
		connectedSocket._childProcessData.id = id;
		connectedSocket._childProcessData.pipeFds[0] = pipeFds[0];
		connectedSocket._childProcessData.pipeFds[1] = pipeFds[1];
		connectedSocket._childProcessData.isError = isError;
}

bool Post::isCgiPostUri(ConnectedSocket &connectedSocket) {
	int index = connectedSocket.getRequestMap()["uri"].find(this->_cgiDirectory);
	if (index == 0)
		return true;
	return false;
}

std::string Post::handlePostCgi(ConnectedSocket &connectedSocket) {

	if (!findScript(connectedSocket, connectedSocket.getRequestMap()["uri"])) {
		// this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(400);
		return this->_responses[connectedSocket.getSocketFd()];
	}

	

	if (connectedSocket.getRequestMap()["uri"] == (_cgiDirectory + this->_cgiFileName)) {
		if (Poll::cgiChildProcessNum >= MAX_CGI_CHILD_PROCESSES)
		{
			this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(503);
			return this->_responses[connectedSocket.getSocketFd()];
		}
		// std::cout << "****cgi*****" << std::endl;
		connectedSocket.setIsCgi(true);
		connectedSocket.setCgiStartTime();
		// this->_responses[connectedSocket.getSocketFd()] = handlePostCgi(connectedSocket.getSocketFd(), connectedSocket);
		connectedSocket._childProcessData= createPipeAndFork(connectedSocket);
		if (connectedSocket._isCgiChildProcessReturning) {
			return "";
		}
		if (connectedSocket._childProcessData.isError)
			return this->_responses[connectedSocket.getSocketFd()];
		else
		{

			this->_responses[connectedSocket.getSocketFd()] = "";
			return this->_responses[connectedSocket.getSocketFd()];
		}
	}
	return "";
}

bool Post::findScript(ConnectedSocket &connectedSocket, std::string &uri) {
	std::cout << "uri = " << uri << std::endl;
	std::string scriptFile = uri.substr(this->_cgiDirectory.length(), std::string::npos);
	this->_cgiFileName = scriptFile;
	std::cout << "scriptFile = " << scriptFile << std::endl;
	if (scriptFile.empty())
		return false;

	// DIR *directory = opendir(this->getStorageDirectory().c_str());
	// if (!directory) {
	// 	this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(500);
	// 	return false;
	// }
	// // std::cout << directory << std::endl;
	// closedir(directory);
	// std::string fileToDelete = this->getStorageDirectory() + "/" + this->_data["filename"];
	// std::cout << YELLOW << "To delete: " << fileToDelete << RESET << std::endl;

	std::string file = "./www/farshad/cgi-post/" + scriptFile;

	int exist = 0;
	int isReadable = 0;
	if ((exist = access(file.c_str(), F_OK)) == 0)
	{
		std::cout << YELLOW << scriptFile << " exists. " << RESET << std::endl;
		this->_cgiFilePath = file;
		std::cout << "this->_cgiFile = " << this->_cgiFilePath << std::endl;
	}
	else
	{
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(404); 
		return (false);
	}
	
	if ((isReadable = access(file.c_str(), R_OK)) == 0)
	{
		std::cout << YELLOW << scriptFile << " is readable." << RESET << std::endl;

		// DIR *directory = opendir(file.c_str());
		// if (directory)
		// {
		// 	Server::logMessage("INFO: " + scriptFile + " is a directory, and not a file!");
		// 	std::cout << YELLOW << scriptFile << " is a directory, and not a file!" << RESET << std::endl;
		// 	closedir(directory);
		// 	this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(400); 
		// 	return false;
		// }
		// else
		// {
		// 	std::cout << YELLOW << scriptFile << " is a file, and not a directory." << RESET << std::endl;
		// 	return true;
		// }
	}
	else {
		std::cout << YELLOW << scriptFile << " is not readable" << RESET << std::endl;
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(403); 
		return false;
	}

	return true;
}


