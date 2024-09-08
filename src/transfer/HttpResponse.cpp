/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:46:45 by nnavidd           #+#    #+#             */
/*   Updated: 2024/09/08 23:55:15 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Get.hpp"
#include "Exception.hpp"
#include "Post.hpp"
#include "Delete.hpp"

HTTPResponse::HTTPResponse() : _storageDirectory("./www/farshad/cloudStorage"), _data(std::map<std::string, std::string>()), _cgiDirectory(""), _cgiFilePath(""), _cgiFileName(""), _queryString(""), _queryStringKeyValues(std::vector<std::string>()) {
	// std::cout << CYAN "HTTPResponse constructor called\n" RESET;
}

HTTPResponse::HTTPResponse(std::map<std::string, std::string> const & serverConfig) :
	_serverConfig(serverConfig)  {
	loadMimeTypes(MIME);
	// std::cout << CYAN "HTTPResponse args constructor called\n" RESET;
}

HTTPResponse::HTTPResponse(std::map<std::string, std::string> const & serverConfig, std::vector<LocationConf> const &locations) :
	_serverConfig(serverConfig), _locations(locations)  {
	loadMimeTypes(MIME);
	setMethods();
	setIndexes();
	// std::cout << CYAN "HTTPResponse args constructor called\n" RESET;
}

HTTPResponse::~HTTPResponse() {
	// std::cout << CYAN "HTTPResponse destructor called\n" RESET;
}

std::string const &HTTPResponse::getStorageDirectory(void) const {
	return this->_storageDirectory;
}

/*Validate The Request Header To Return The Corrsponding Status Code.*/
int HTTPResponse::validate() {
	if (_requestMap.find("Host") == _requestMap.end()) {
		Server::logMessage("ERROR: The HOST Is Wrong!");
		return (400);
	// if (_requestMap.find(""))
	}
	return (200);
}

// bool HTTPResponse::isDirectory(const std::string& uri) const {
// 	std::string filePath = _serverConfig.at("root") + uri;
// 	struct stat st;
// 	if (stat(filePath.c_str(), &st) != 0) {
// 		return false; // Error in accessing the path or path does not exist
// 	}
// 	return S_ISDIR(st.st_mode);
// }

bool HTTPResponse::isDirectory(const std::string& uri) const {
    std::string filePath = _serverConfig.at("root") + uri;
    DIR* dir = opendir(filePath.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

/*Check whether the passed file path is directory or not.*/
bool HTTPResponse::isFile(const std::string &filePath) {
    int fd = open(filePath.c_str(), O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        // If errno is ENOTDIR, the path is a regular file
        return (errno == ENOTDIR);
    } else {
        // It's a directory, close the file descriptor
        close(fd);
        return false;
    }
}

// bool HTTPResponse::isFile(std::string const & filePath) {
//     struct stat st;
// 	if (stat(filePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
// 		return (false);
// 	return (true);
// }

/*Check the the uri, and add '/' at the end of directory uri that is received without it.*/
void HTTPResponse::fixUri(std::string const &filePath) {
    // If it's not a file, fix the URI
    if (!isFile(filePath)) {
        // Safely access the last character manually
        if (_requestMap["uri"][_requestMap["uri"].size() - 1] != '/') {
            _requestMap["uri"] += "/";
        } else {
            // If the URI ends with multiple '/', reduce it to just one
            while (_requestMap["uri"][_requestMap["uri"].size() - 2] == '/') {
                _requestMap["uri"].erase(_requestMap["uri"].size() - 1);  // Remove extra slashes
            }
        }
    }
    // std::cout << "uri2: " << _requestMap["uri"] << std::endl;
}

/*Return Corresponding Status Code Response Or In Case
Of Responding With An Specific Method Is Required,
It Invokes Corresponding Method.*/
std::string HTTPResponse::getResponse(int const clientSocket, ConnectedSocket &connectedSocket) {
	int statusCode = validate();

	std::string method = _requestMap["method"];
	std::string uri = _requestMap["uri"];
	std::string filePath = _serverConfig.at("root") + uri;
	
	// printRequestMap();
	// printServerConfig();
	if (statusCode == 400) {
		return generateErrorPage(400);
	}
	if (statusCode == 304) {
		return generateErrorPage(304);
	}
	// std::cout << "URI1: " << uri << std::endl;
	fixUri(filePath);
	uri = _requestMap["uri"];
	// printMethods();
	// std::cout << "URI3: " << uri << std::endl;
	// std::cout << BLUE << splitLocationFromUri(uri) << RESET << std::endl;
	if (getLocationMethod(uri) != "") {
		if (getLocationMethod(uri).find(method) == std::string::npos)
			return (generateErrorPage(405));
	}
		// std::cout << MAGENTA <<getLocationMethod(uri)<< RESET << std::endl;

	// First, check if the method is GET or HEAD
	if (method == "GET" || method == "HEAD") {
		return createHandleGet(connectedSocket);
	}

	// Then, check if the method is POST or DELETE and the URI is not a directory
	if ((method == "POST" || method == "DELETE")) {// && !isDirectory(uri)) {
		if (method == "POST" && this->_requestMap["uri"] == "/delete")
			return createHandleDelete(connectedSocket);
		else if (method == "POST")
			return createHandlePost(clientSocket, connectedSocket, this->_serverConfig);
		else if (method == "DELETE")
			return createHandleDelete(connectedSocket);
	}

	// If none of the above conditions are met, return a 405 Method Not Allowed error
	return generateErrorPage(405);
}


/*Creat An Instance of Get Class And
Call The Get Method To Prepare The Response.*/
std::string HTTPResponse::createHandleGet(ConnectedSocket &connectedSocket) {
	Get  Get(_requestMap, _serverConfig, this->_locations);
	return (Get.handleGet(connectedSocket));
}

std::string HTTPResponse::createHandlePost(int const connectedSocketFd, ConnectedSocket &connectedSocket, std::map<std::string, std::string> &serverConfig) {
	Post postResponse;
	postResponse.handlePost(connectedSocketFd, connectedSocket, serverConfig);
	if (connectedSocket.getIsCgiChildProcessReturning() == true)
		return "";
	std::string response = postResponse.getSocketResponse(connectedSocketFd);
	postResponse.removeSocketResponse(connectedSocketFd);
	postResponse.clearData();
	return (response);
}

std::string HTTPResponse::createHandleDelete(ConnectedSocket &connectedSocket) {
	// std::string responseBody = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n\r\n<html><body><h1>DELETE Request Received</h1></body></html>";
	Delete deleteResponse;
	deleteResponse.handleDelete(connectedSocket);
	std::string response = deleteResponse.getSocketResponse(connectedSocket.getSocketFd());

	deleteResponse.removeSocketResponse(connectedSocket.getSocketFd());
	deleteResponse.clearData();
	return response;
}

/*Return Message Corresponding To The Status Code Is Passed.*/
std::string HTTPResponse::httpStatusCode(int statusCode) {
	switch (statusCode) {
		case 200: return "HTTP/1.1 200 OK";
		case 304: return "HTTP/1.1 304 Not Modified";
		case 302: return "HTTP/1.1 302 Found";
		case 400: return "HTTP/1.1 400 Bad Request";
		case 403: return "HTTP/1.1 403 Forbidden";
		case 404: return "HTTP/1.1 404 Not Found";
		case 405: return "HTTP/1.1 405 Method Not Allowed";
		case 413: return "HTTP/1.1 413 Payload Too Large";
		case 503: return "HTTP/1.1 503 Service Unavailable";
		case 501: return "HTTP/1.1 501 Not Implemented";
		case 504: return "HTTP/1.1 504 Gateway Timeout";
		default:  return "HTTP/1.1 500 Internal Server Error";
	}
}

/*Reading The Binary From The Path and Return a String*/
std::string HTTPResponse::readBinaryFile(std::string const & path) {
	std::ifstream fileStream(path.c_str(), std::ios::binary);
	if (!fileStream.is_open()) {
		Server::logMessage("ERROR: File Not Open In The readBinaryFile Function!");
		// perror("error:");
		return "";
	}
	std::ostringstream ss;
	ss << fileStream.rdbuf();
	fileStream.close();
	return ss.str();
}

/*Reading From The Path and Return a String*/
std::string HTTPResponse::readHtmlFile(const std::string &path) {
	std::ifstream fileStream(path.c_str());
	if (!fileStream.is_open()) {
		Server::logMessage("ERROR: File Not Open In The readHtmlFile Function!");
		perror("error:");
		return "";
	}
	std::ostringstream ss;
	ss << fileStream.rdbuf();
	fileStream.close();
	return ss.str();
}

// Helper function to check if the file exists and is executable
bool isExecutable(const std::string& path) {
    return (access(path.c_str(), X_OK) == 0);  // Check if executable
}

// Helper function to split PATH by ':'
std::vector<std::string> splitPath(const std::string& path) {
    std::vector<std::string> paths;
    std::stringstream ss(path);
    std::string directory;

    while (std::getline(ss, directory, ':')) {
        paths.push_back(directory);
    }
    return paths;
}

// Main function to set the interpreter based on the file extension
std::string setInterpreter(const std::string& ext) {
    const char* pathEnv = std::getenv("PATH");

    if (!pathEnv) {
        std::cerr << "ERROR: PATH environment variable not found!" << std::endl;
        return "";
    }

    std::vector<std::string> paths = splitPath(pathEnv);

    std::string interpreter;
    if (ext == ".py") {
        interpreter = "python3";
    // } if (ext == ".pl") {
    //     interpreter = "perl";
    } else {
        interpreter = "bash";  // Default interpreter
    }

    // Search in each directory in the PATH
    for (std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); ++it) {
        std::string fullPath = *it + "/" + interpreter;
        if (isExecutable(fullPath)) {
            return fullPath;  // Found executable interpreter
        }
    }

    // If not found in PATH, fallback to default paths
	// here we can add new cgi command language
    if (ext == ".py") {
        return "/usr/bin/python";
    // } else if (ext == ".pl") {
    //     return "/usr/bin/perl";
    } else if (ext == ".sh") {
        return "/bin/bash";  // Default to bash
    } else
		return "";
}


/*Return a String Of Current Time In a HTTP Header Format.*/
std::string HTTPResponse::formatTimeHTTP(std::time_t rawTime) {
	std::tm *gmTime = std::gmtime(&rawTime);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmTime);
	return (std::string(buffer));
}

std::string HTTPResponse::getCurrentTime() {
	std::time_t currentTime = std::time(NULL);
	return formatTimeHTTP(currentTime);
}

/*Generate ETag According To The FilePath Last Modifed And Size.*/
std::string HTTPResponse::generateETag(const std::string &filePath, std::string &date, std::string &lastModified) {
	struct stat fileInfo;

	if (stat(filePath.c_str(), &fileInfo) != 0) {
		Server::logMessage("Error: Getting File Information In generateETag function Failed: " + filePath);
		// std::cerr << "Error getting file information: " << filePath << std::endl;
		return "";
	}

	std::ostringstream etag;
	etag << "\"" << fileInfo.st_mtime << "-" << fileInfo.st_size << "\"";

	date = getCurrentTime();
	lastModified = formatTimeHTTP(fileInfo.st_mtime);

	return etag.str();
}

/*This Function Is Called Two Times, First After POLLIN To Generate
The Response And Assign It To Its Socket In A Map Of Responses,
Second After POLLOUT To Grab The Corresponding Response To The Socket
And Sent It To Client. It Returns False If There Is No Response For Specific
Socket Or There Is No POLLIN Or POLLOUT, Otherwise Remove The Sent Response
From The Response Map And Return True. */

bool HTTPResponse::handleResponse(int clientSocket, int const &pollEvent, pollfd *pollFds, size_t i, ConnectedSocket &connectedSocket) {
	if (pollEvent == POLLIN_TMP) {
		if (connectedSocket.getIsCgi() && this->_responses[clientSocket].empty()) {
			return true;
		}
		if (connectedSocket.getState() == ERROR)
			_responses[clientSocket] = generateErrorPage(400);
		else
			_responses[clientSocket] = getResponse(clientSocket, connectedSocket);
		Server::logMessage("INFO: Response Generated for socket fd: " + Server::intToString(clientSocket));
		return true;
	}
	if (pollEvent == POLLOUT_TMP) {
		std::map<int, std::string>::iterator iter = _responses.find(clientSocket);
		if (iter == _responses.end()) {
			Server::logMessage("Error: No response In The _responses found for socket fd: " + Server::intToString(clientSocket));
			return false;
		}
		std::string response = iter->second;
		// printStringToFile(response, "./src/request/response.txt");

		// std::cout << CYAN << "Connected socket " << clientSocket << " is sending the response ..." << RESET << std::endl;

		ssize_t bytesSent = send(clientSocket, this->_responses[clientSocket].c_str(), this->_responses[clientSocket].size(), 0);

		// std::cout <<  "******" << bytesSent << "******" << std::endl;
		if (bytesSent == -1) {
			Server::logMessage("Error: No Byte Sent for socket fd: " + Server::intToString(clientSocket));
			return false;
		}
		// std::cout << CYAN << "Bytes sent: " << bytesSent << RESET << std::endl;
		// std::cout << CYAN << "Response size: " << this->_responses[clientSocket].size() << RESET << std::endl;

		connectedSocket.setConnectionStartTime();

		// if (bytesSent < static_cast<ssize_t>(this->_responses[clientSocket].size()) || (connectedSocket.getRequestMap()["uri"] == "/cgi-post")) {
		if (bytesSent < static_cast<ssize_t>(this->_responses[clientSocket].size())) {
			Server::logMessage("WARNING: Sent Byte Less Than The Response for socket fd: " + Server::intToString(clientSocket));
			pollFds[i].events = POLLOUT;
			this->_responses[clientSocket].erase(0, bytesSent);
			connectedSocket.setState(WRITING);
		}
		else {
			Server::logMessage("INFO: Response Sent for socket fd: " + Server::intToString(clientSocket));
			connectedSocket.setState(DONE);
			pollFds[i].events = POLLIN;
			pollFds[i].revents = 0;
			_responses.erase(clientSocket);
		}
		return true;
	}
	Server::logMessage("ERROR: Response Function Failed for socket fd: " + Server::intToString(clientSocket));
	return false;
}
// HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>

/*Display Corresponding Response To The Fd Is Passed.*/
void HTTPResponse::printSocketResponse(int fd) {
	std::cout << ORG "****Response for fd [" << fd << "] is:\n" << MAGENTA << _responses[fd] << RESET << std::endl;
}

/*Print A String In Passed File Path.*/
void HTTPResponse::printStringToFile(const std::string& string, const std::string& path) {
	// std::cout << RED "****Printing response in file: " BLUE << path << RESET << std::endl;
	std::ofstream outfile(path.c_str());
	if (!outfile.is_open()) {
		Server::logMessage("ERROR: File Opening Failed In printStringToFile For This Path: " + path);
		return ;
	}
	outfile << string << std::endl;
	outfile.close();
}

/*Set The Map Request Result Of Request Handling In HTTP Response Class.*/
void HTTPResponse::setRequestMapInResponse(const std::map<std::string, std::string> & requestMap) {
	_requestMap = requestMap;
}

/*Set The String Request Result Of Request Handling In HTTP Response Class.*/
void HTTPResponse::setRequestStringInResponse(std::string const & requestString) {
	_requestString = requestString;
}

/*Read From The MIME FilePath, And Creat A Map Of MIME Types.*/
void HTTPResponse::loadMimeTypes(const std::string& filePath) {
	std::ifstream file(filePath.c_str());
	if (!file) {
		Server::logMessage("ERROR: MIME Read Failed: " + filePath);
		std::cerr << "Error opening MIME types file: " << filePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string extension, mimeType;
		if (!(iss >> extension >> mimeType)) {
			std::cerr << "Invalid format in MIME types file: " << line << std::endl;
			continue;
		}
		_mimeMap[extension] = mimeType;
	}
	Server::logMessage("INFO: MIME Loaded.");
}

/*Receive The Extention, And Retrieve The Corresponding MIME Type From The MIME Map Variable.*/
std::string HTTPResponse::getMimeType(const std::string& extension) const {
	std::map<std::string, std::string>::const_iterator it = _mimeMap.find(extension);
	if (it != _mimeMap.end()) {
		return it->second;
	} else {
		return "application/octet-stream"; // Default MIME type
	}
}

void HTTPResponse::printRequestMap()
{
	std::cout << RED "****Request Map Inside HTTPResponse:\n";
	std::map<std::string, std::string>::iterator itr = _requestMap.begin();
	for (; itr != _requestMap.end(); itr++)
		std::cout << ORG << itr->first << ":" MAGENTA << itr->second << RESET << std::endl;
}

void HTTPResponse::printServerConfig()
{
	std::cout << RED "****The server config map Inside HTTPResponse:\n";
	std::map<std::string, std::string>::iterator itr = _serverConfig.begin();
	for (; itr != _serverConfig.end(); itr++)
		std::cout << ORG << itr->first << "->" MAGENTA << itr->second << RESET << std::endl;
}

void HTTPResponse::printData(void) {
	if (!_data.size())
	{
		std::cout << YELLOW << "DATA IS EMPTY" << RESET << std::endl;
		return;
	}

	std::map<std::string, std::string>::iterator iterator = this->_data.begin();

	std::cout << YELLOW << "DATA RECEIVED:" << RESET <<std::endl;

	while (iterator != this->_data.end())
	{
		std::cout << YELLOW << iterator->first << ": " << iterator->second << RESET << std::endl;
		iterator++;
	}
}

void HTTPResponse::printResponsesMap(void) {
	if (!this->_responses.size())
	{
		std::cout << BLUE << "RESPONSES MAP IS EMPTY" << RESET << std::endl;
		return;
	}
	std::map<int, std::string>::iterator iterator;
	std::map<int, std::string>::iterator iteratorEnd = this->_responses.end();

	for (iterator = this->_responses.begin(); iterator != iteratorEnd; iterator++)
	std::cout << BLUE << "Connected socket [" << iterator->first << "] sends the response:\n" << iterator->second << RESET << std::endl;
	return;
}

void HTTPResponse::removeSocketResponse(int connectedSocketFd) {
	this->_responses.erase(connectedSocketFd);
}

void HTTPResponse::clearData(void) {
	this->_data.clear();
}

std::string const &HTTPResponse::getSocketResponse(int connectedSocketFd) {
	return this->_responses[connectedSocketFd];
}

std::string HTTPResponse::getSubStringFromMiddleToIndex(std::string &string, std::string const &toFind, size_t startOffset, size_t endIndex) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(foundIndex + startOffset, endIndex);
	return result;
}

std::string HTTPResponse::getSubStringFromStartToIndex(std::string &string, std::string const &toFind) {
	size_t foundIndex = string.find(toFind);
	if (foundIndex == std::string::npos)
		return "";
	std::string result = string.substr(0, foundIndex);
	return result;
}
std::string HTTPResponse::generateErrorHeaders(int statusCode, size_t contentLength) {
	std::ostringstream headers;
	headers << httpStatusCode(statusCode) << CRLF // Status line
			<< "Server: " << _serverConfig["server_name"] << CRLF
			<< "Connection: Keep-Alive" << CRLF
			<< "Content-Type: text/html" << CRLF;
	if (contentLength) {
		headers << "Content-Length: " << contentLength << CRLF;
	}
		headers << CRLF;
	Server::logMessage("INFO: Error Header Created, StatusCode: " + Server::intToString(statusCode) );
	return headers.str();
}

/*Generate the default error the corresponding error page doesn't exist.*/
std::string HTTPResponse::generateDefaultErrorPage(int statusCode, std::string const & message) {
	Server::logMessage("INFO: Default Error Body Dynamically Generated, StatusCode: " + Server::intToString(statusCode));
	// Replace with your custom error page logic
	std::string content = "<html>\r\n<head><title>" + Server::intToString(statusCode) + " " + message + 
		"</title></head>\r\n<body>\r\n<center><h2>" + Server::intToString(statusCode) + " "	+ message +
		"</h2></center>\r\n<hr><center>Musketeers Group!</center>\r\n</body>\r\n</html>";
	
	return content;
}

/*Prepare the corresponding error by reading its error page, otherwise 
call the  generateDefaultErrorPage function.*/
std::string HTTPResponse::generateErrorPage(int statusCode) {
	std::string errorFilePath = "./src/transfer/errors/" + Server::intToString(statusCode) + ".html";

	std::ifstream file(errorFilePath.c_str());
	if (file.is_open()) {
		Server::logMessage("INFO: Default Error Page Statically Read, StatusCode: " + Server::intToString(statusCode));
		// Custom error page exists
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		size_t contentLength = content.length();
		std::string headers = generateErrorHeaders(statusCode, contentLength);
		return headers + readBinaryFile(errorFilePath);
	} else {
		// Default error page
		std::string message = httpStatusCode(statusCode).substr(13, httpStatusCode(statusCode).find(CRLF));
		std::string data = generateDefaultErrorPage(statusCode, message);
		size_t contentLength = data.size();
		std::string headers = generateErrorHeaders(statusCode, contentLength);
		return headers + data;
	}
}

void HTTPResponse::setResponseForAConnectedSocket(std::string const &response, int connectedSocketFd) {
	this->_responses[connectedSocketFd] = response;
}



//////////////////////// CGI ////////////////////////



ChildProcessData HTTPResponse::createPipeAndFork(ConnectedSocket &connectedSocket) {
	int pipeFds[2];
	// int stdInCopy = dup(STDIN_FILENO);
	// int stdOutCopy = dup(STDOUT_FILENO);

	if (pipe(pipeFds) == -1) {
		cgiError(connectedSocket);
		return connectedSocket.getChildProcessData() ;
	}

	Server::logMessage("pipeFds[0] = " + Server::intToString(pipeFds[0]));
	Server::logMessage("pipeFds[1] = " + Server::intToString(pipeFds[1]));

	pid_t id = fork();
	if (id == -1) {
		close(pipeFds[0]);
		close(pipeFds[1]);
		this->cgiError(connectedSocket);
		return connectedSocket.getChildProcessData();
	}

	Poll::cgiChildProcessNum++;
	std::cout << "Poll::cgiChildProcessNum = " << Poll::cgiChildProcessNum << std::endl;

	if (id == 0) {		
		this->handleCgiChildProcess(connectedSocket, pipeFds);
		return connectedSocket.getChildProcessData();
	} else {
		handleCgiMainProcess(connectedSocket, pipeFds, id);
	 	return connectedSocket.getChildProcessData();;
	}
	return connectedSocket.getChildProcessData();;
}

void HTTPResponse::cgiError(ConnectedSocket &connectedSocket) {
	this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(500);
	int fds[2] = {-1, -1};
	this->UpdateCgiProperties(connectedSocket, -1, fds, true);
	connectedSocket.setState(DONE);
	return;
}

void HTTPResponse::handleCgiChildProcess(ConnectedSocket &connectedSocket, int pipeFds[2]) {
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

	std::string command = setInterpreter(connectedSocket.getCgiScriptExtension());
	// if (connectedSocket.getCgiScriptExtension() == ".sh")
	// 	command = "/bin/bash";
	// else if (connectedSocket.getCgiScriptExtension() == ".py")
	// 	command = "/usr/bin/python3";
	if (command == "") {
		connectedSocket.setIsCgiChildProcessReturning(true);
		return;
	}

	if (connectedSocket.getRequestMap()["method"] == "POST") {
		Post post;
		post.parsePostRequest(connectedSocket.getRequestHeader(), connectedSocket.getRequestBody());
		std::string requestData = post._data["name"];
		std::string keyValue = "NAME=" + requestData;
		// std::cerr << "keyValue = " << keyValue << std::endl;
		char *name = const_cast<char *>(keyValue.c_str());
		char *const env[] = {name, NULL};

		// std::string command = this->findCommand("node");

		// std::string command = "/Users/fahmadia/.nvm/versions/node/v20.15.0/bin/node";
		// std::string command = "/usr/bin/python";
		char *cmd = const_cast<char *>(command.c_str());

		// std::string file = "./www/farshad/form/cgi.js";
		std::string file = this->_cgiFilePath;
		char *filePath = const_cast<char *>(file.c_str());

		char *const argv[] = {cmd, filePath, NULL};

		execve(cmd, argv, env);
	}
	else if (connectedSocket.getRequestMap()["method"] == "GET") {
		this->storeKeyValuePairsOfQueryString();
		// this->printQueryStringKeyValues();
		char **env = this->getEnv();
		// printEnv(env);

		// std::string command = "/usr/bin/python";
		char *cmd = const_cast<char *>(command.c_str());

		std::string file = this->_cgiFilePath;
		char *filePath = const_cast<char *>(file.c_str());

		char *const argv[] = {cmd, filePath, NULL};
		std::cerr << "cmd = " << cmd << std::endl;
		std::cerr << "argv[1] = " << argv[1] << std::endl;
		this->resetCgiProperties();
		execve(cmd, argv, env);
		this->deleteChildProcessMemory(env);
	}


	// std::cerr << RED << "cmd or argv are wrong => execve failed. execve() returned: " << result << RESET << std::endl;
	connectedSocket.setIsCgiChildProcessReturning(true);
	return;

}

void HTTPResponse::handleCgiMainProcess(ConnectedSocket &connectedSocket, int pipeFds[2], pid_t id) {
	// dup2(pipeFds[0], STDIN_FILENO);
	this->UpdateCgiProperties(connectedSocket, id, pipeFds, false);

	if (connectedSocket.getChildProcessData().pipeFds[1] != -1)
	{
		if (close(pipeFds[1]) == -1) {
		connectedSocket.getChildProcessData().pipeFds[1] = -1;
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

void HTTPResponse::UpdateCgiProperties(ConnectedSocket &connectedSocket, pid_t id, int pipeFds[2], bool isError) {
		connectedSocket.getChildProcessData().id = id;
		connectedSocket.getChildProcessData().pipeFds[0] = pipeFds[0];
		connectedSocket.getChildProcessData().pipeFds[1] = pipeFds[1];
		connectedSocket.getChildProcessData().isError = isError;
}

bool HTTPResponse::isCgiUri(ConnectedSocket &connectedSocket) {
	// std::cout << "uri = " << connectedSocket.getRequestMap()["uri"] << std::endl;
	int index = connectedSocket.getRequestMap()["uri"].find(this->_cgiDirectory);
	// std::cout << "IN isCgiUri index is:" << index << std::endl;
	if (index == 0)
		return true;
	return false;
}

std::string HTTPResponse::handleCgi(ConnectedSocket &connectedSocket) {

	if (!findScript(connectedSocket, connectedSocket.getRequestMap()["uri"])) {
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(404);
		return this->_responses[connectedSocket.getSocketFd()];
	}

	if (getScriptExtension(connectedSocket).empty()) {
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(501);
		return this->_responses[connectedSocket.getSocketFd()];
	}


	if(!isScriptExtensionValid(connectedSocket)) {
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(501);
		return this->_responses[connectedSocket.getSocketFd()];
	}

	// if (connectedSocket.getRequestMap()["uri"] == (_cgiDirectory + this->_cgiFileName)) {
	if (connectedSocket.getRequestMap()["uri"].find(_cgiDirectory + this->_cgiFileName) != std::string::npos) {
		if (Poll::cgiChildProcessNum >= MAX_CGI_CHILD_PROCESSES)
		{
			this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(503);
			return this->_responses[connectedSocket.getSocketFd()];
		}
		// std::cout << "****cgi*****" << std::endl;
		connectedSocket.setIsCgi(true);
		connectedSocket.setCgiStartTime();
		// this->_responses[connectedSocket.getSocketFd()] = handlePostCgi(connectedSocket.getSocketFd(), connectedSocket);
		ChildProcessData childProcessData = createPipeAndFork(connectedSocket);
		connectedSocket.setChildProcessData(childProcessData);
		if (connectedSocket.getIsCgiChildProcessReturning() == true) {
			return "";
		}
		if (connectedSocket.getChildProcessData().isError) {

			return this->_responses[connectedSocket.getSocketFd()];
		}
		else
		{

			this->_responses[connectedSocket.getSocketFd()] = "";
			return this->_responses[connectedSocket.getSocketFd()];
		}
	}
	return "";
}

bool HTTPResponse::findScript(ConnectedSocket &connectedSocket, std::string &uri) {
	// std::cout << "uri = " << uri << std::endl;
	if (connectedSocket.getRequestMap()["method"] == "POST") {
		std::string scriptFile = uri.substr(this->_cgiDirectory.length(), std::string::npos);
		this->_cgiFileName = scriptFile;
		// std::cout << "scriptFile = " << this->_cgiFileName << std::endl;
	} else if (connectedSocket.getRequestMap()["method"] == "GET") {
		std::string scriptFile = "";
		size_t queryStringStartIndex = uri.find_first_of("?");
		if (queryStringStartIndex != std::string::npos)
		{
			std::string temp = uri.substr(this->_cgiDirectory.length(), std::string::npos);
			scriptFile = this->getSubStringFromStartToIndex(temp, "?");
			this->_queryString = uri.substr(queryStringStartIndex + 1, std::string::npos);
			// std::cout << "queryStrig = " << this->_queryString << std::endl;
		}
		else
			scriptFile = uri.substr(this->_cgiDirectory.length(), std::string::npos);

		this->_cgiFileName = scriptFile;
		std::cout << "scriptFile = " << this->_cgiFileName << std::endl;
	}
	if (this->_cgiFileName.empty())
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

	std::string file = "";
	if (connectedSocket.getRequestMap()["method"] == "POST")
		file = "./www/farshad/cgi-post/" + this->_cgiFileName;
	else if (connectedSocket.getRequestMap()["method"] == "GET")
		file = _serverConfig.at("root") + "/cgi-get/" + this->_cgiFileName;
	std::cout << "file address :" << file << std::endl;
	int exist = 0;
	int isReadable = 0;
	if ((exist = access(file.c_str(), F_OK)) == 0)
	{
		std::cout << YELLOW << this->_cgiFileName << " exists. " << RESET << std::endl;
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
		std::cout << YELLOW << this->_cgiFileName << " is readable." << RESET << std::endl;

		// DIR *directory = opendir(file.c_str());
		// if (directory)
		// {
		// 	Server::logMessage("INFO: " + this->_cgiFileName + " is a directory, and not a file!");
		// 	std::cout << YELLOW << this->_cgiFileName << " is a directory, and not a file!" << RESET << std::endl;
		// 	closedir(directory);
		// 	this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(400); 
		// 	return false;
		// }
		// else
		// {
		// 	std::cout << YELLOW << this->_cgiFileName << " is a file, and not a directory." << RESET << std::endl;
		// 	return true;
		// }
	}
	else {
		std::cout << YELLOW << this->_cgiFileName << " is not readable" << RESET << std::endl;
		this->_responses[connectedSocket.getSocketFd()] = generateErrorPage(403); 
		return false;
	}

	return true;
}

void HTTPResponse::storeKeyValuePairsOfQueryString(void) {
	// std::cerr << RED << "queryString = " << this->_queryString << RESET << std::endl;
	// std::cerr << RED << "scriptFile = " << this->_cgiFileName << RESET << std::endl;

	// std::vector<std::string> keyValues;
	std::string queryString = this->_queryString;
	while (!queryString.empty())
	{
		if (queryString.find("&") == std::string::npos) {
			// std::cerr << "*** " << queryString << std::endl;
			this->_queryStringKeyValues.push_back(queryString);
		} else {
			std::string keyValue = this->getSubStringFromStartToIndex(queryString, "&");
			if (!keyValue.empty()) {
				// std::cerr << BLUE << keyValue << RESET << std::endl;
				this->_queryStringKeyValues.push_back(keyValue);
			}
		}
		queryString = this->getSubStringFromMiddleToIndex(queryString, "&", 1, std::string::npos);
		// std::cerr << YELLOW << queryString << RESET << std::endl;
	}
}

void HTTPResponse::resetCgiProperties(void) {
	this->_cgiFilePath = "";
	this->_cgiFileName = "";
	this->_queryString = "";
	this->_queryStringKeyValues.clear();
}

void HTTPResponse::printQueryStringKeyValues(void) {
	std::vector<std::string>::iterator iterator;
	std::vector<std::string>::iterator iteratorEnd = this->_queryStringKeyValues.end();
	
	std::cerr << "Query String Key-Values:" << std::endl;
	for (iterator = this->_queryStringKeyValues.begin(); iterator != iteratorEnd; iterator++) {
	std::cerr << *iterator << std::endl;
	}
}

char **HTTPResponse::getEnv(void) {
	std::vector<std::string>::iterator iterator;
	std::vector<std::string>::iterator iteratorEnd = this->_queryStringKeyValues.end();

	// std::cerr << "???" << this->_queryStringKeyValues.size() << "--" << std::endl;
	char **env = new char*[this->_queryStringKeyValues.size() + 1];
	env[this->_queryStringKeyValues.size()] = NULL;
	// std::cerr << BLUE << "** == " << env[3] << RESET << std::endl;
	// std::cerr << RED << "_queryStringKeyValues.size() = " << _queryStringKeyValues.size() << RESET << std::endl;
	int i = 0;
	for (iterator = this->_queryStringKeyValues.begin(); iterator != iteratorEnd; iterator++) {
		char *keyValue = new char[iterator->length() + 1];
		keyValue[iterator->length()] = '\0';
		env[i] = const_cast<char *>(iterator->c_str());
		// std::cerr << "*** i: " << i << " " << env[i] << std::endl;
		i++;
	}
	return env;
}

void HTTPResponse::printEnv(char **env) {
	int i = 0;
	while (*env) {
		std::cerr << "env[" << i << "] = " << *env << std::endl;
		env++;
		i++;
	}
}

bool HTTPResponse::isScriptExtensionValid(ConnectedSocket &connectedSocket) {
	std::string validExtensions[VALID_EXTENSIONS_NUM] = {".sh", ".py"};
	
	for (size_t i = 0; i < VALID_EXTENSIONS_NUM; i++) {
		if (connectedSocket.getCgiScriptExtension() == validExtensions[i]) {
			return true;
		}
	}
	return false;
}

std::string HTTPResponse::getScriptExtension(ConnectedSocket &connectedSocket) {
	size_t index = (this->_cgiFileName).find_last_of(".");
	if (index == std::string::npos)
		return "";
	std::string scriptExtension = this->_cgiFileName.substr(index, std::string::npos);
	std::cout << "EXTENSION: " << scriptExtension << std::endl;
	connectedSocket.setCgiScriptExtension(scriptExtension);
	std::cerr << "-----extension = " <<connectedSocket.getCgiScriptExtension() << std::endl;
	return scriptExtension;
}

void HTTPResponse::deleteChildProcessMemory(char **env) {
	char **temp = env;
	while (*temp) {
		delete[] *temp;
		temp++;
	}
	delete []env;
}

void HTTPResponse::setMethods(void) {
	std::vector<LocationConf>::iterator iterator;
	std::vector<LocationConf>::iterator iteratorEnd = this->_locations.end();

	for (iterator = this->_locations.begin(); iterator != iteratorEnd; iterator++) {
		if ((iterator->getSettings().find("method") != iterator->getSettings().end()) && (iterator->getSettings().find("uri") != iterator->getSettings().end())) {
			const std::string uri = iterator->getASettingValue("uri");
			const std::string method = iterator->getASettingValue("method");
			this->_methods[uri] = method;
		}
	}
}

void HTTPResponse::setIndexes(void) {
	std::vector<LocationConf>::iterator iterator;
	std::vector<LocationConf>::iterator iteratorEnd = this->_locations.end();

	for (iterator = this->_locations.begin(); iterator != iteratorEnd; iterator++) {
		if ((iterator->getSettings().find("index") != iterator->getSettings().end()) && (iterator->getSettings().find("uri") != iterator->getSettings().end())) {
			const std::string uri = iterator->getASettingValue("uri");
			const std::string index = iterator->getASettingValue("index");
			this->_indexes[uri] = index;
		}
	}
}

void HTTPResponse::printMethods(void) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_methods.end();

	std::cout << RED "****The location accepted methods:" RESET << std::endl;

	for (iterator = this->_methods.begin(); iterator != iteratorEnd; iterator++) {
		std::cout << GREEN << iterator->first << RESET  " = " ORG << iterator->second << RESET << std::endl;
	}
}

void HTTPResponse::printIndexes(void) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = this->_indexes.end();

	std::cout << RED "****The location indexes:" RESET << std::endl;

	for (iterator = this->_indexes.begin(); iterator != iteratorEnd; iterator++) {
		std::cout << GREEN << iterator->first << RESET  " = " ORG << iterator->second << RESET << std::endl;
	}
}

std::string HTTPResponse::splitLocationFromUri(const std::string& path) {
    // Check if the path is only "/"
    if (path == "/") {
        return path; // Return "/" as is if it's alone
    }

    // Find the first occurrence of "/" after the first one
    size_t secondSlashPos = path.find('/', 1);

    // If there's no other "/", return the entire path
    if (secondSlashPos == std::string::npos) {
        return "/";
    }

    // Otherwise, return the substring up to the second "/"
    return path.substr(0, secondSlashPos);
}

std::string const HTTPResponse::getLocationMethod(std::string const & uri) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = _methods.end();
	std::string location = splitLocationFromUri(uri);

	for (iterator = _methods.begin(); iterator != iteratorEnd; iterator++) {
		if (iterator->first == location)
			return (iterator->second);
	}
	return ("");
}

std::string const HTTPResponse::getLocationIndex(std::string const & uri) {
	std::map<std::string, std::string>::iterator iterator;
	std::map<std::string, std::string>::iterator iteratorEnd = _indexes.end();
	std::string location = splitLocationFromUri(uri);

	for (iterator = _indexes.begin(); iterator != iteratorEnd; iterator++) {
		if (iterator->first == location)
			return (iterator->second);
	}
	return ("");
}