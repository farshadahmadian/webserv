/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:46:45 by nnavidd           #+#    #+#             */
/*   Updated: 2024/08/13 16:04:04 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "GetHandler.hpp"
#include "Exception.hpp"

HTTPResponse::HTTPResponse() {
	// std::cout << CYAN "HTTPResponse constructor called\n" RESET;
}

HTTPResponse::HTTPResponse(std::map<std::string, std::string> const & serverConfig) :
	_serverConfig(serverConfig), _post(Post())  {
	loadMimeTypes(MIME);
	// std::cout << CYAN "HTTPResponse args constructor called\n" RESET;
}

HTTPResponse::~HTTPResponse() {
	// std::cout << CYAN "HTTPResponse destructor called\n" RESET;
}

/*Validate The Request Header To Return The Corrsponding Status Code.*/
int HTTPResponse::validate() {
	if (_requestMap.find("Host") == _requestMap.end()) {
		Server::logMessage("The HOST Is Wrong!");
		return (400);
	// if (_requestMap.find(""))
	}
	return (200);
}

bool HTTPResponse::isDirectory(const std::string& uri) const {
    std::string filePath = _serverConfig.at("root") + uri;
    struct stat st;
    if (stat(filePath.c_str(), &st) != 0) {
        return false; // Error in accessing the path or path does not exist
    }
    return S_ISDIR(st.st_mode);
}

/*Return Corresponding Status Code Response Or In Case
Of Responding With An Specific Method Is Required,
It Invokes Corresponding Method.*/
std::string HTTPResponse::getResponse(int const clientSocket) {
    int statusCode = validate();

    std::string method = _requestMap["method"];
    std::string uri = _requestMap["uri"];

    // displayRequestMap();
	// displayServerConfig();
    if (statusCode == 400) {
        return generateErrorPage(400);
    }
    if (statusCode == 304) {
        return generateErrorPage(304);
    }

    // First, check if the method is GET or HEAD
    if (method == "GET" || method == "HEAD") {
        return createHandleGet();
    }

    // Then, check if the method is POST or DELETE and the URI is not a directory
    if ((method == "POST" || method == "DELETE") && !isDirectory(uri)) {
        if (method == "POST") {
            return createHandlePost(clientSocket);
        } else if (method == "DELETE") {
            return createHandleDelete();
        }
    }

    // If none of the above conditions are met, return a 405 Method Not Allowed error
    return generateErrorPage(405);
}

// std::string HTTPResponse::getResponse(int const clientSocket) {
// 	int statusCode = validate();

// 	std::string method = _requestMap["method"];
// 	// std::cout << RED "****received method is: " BLUE << method << RESET << std::endl;
// 	displayRequestMap();
// 	if (statusCode == 400) {
// 		// return (httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>");
// 		return (generateErrorPage(400));
// 	}
// 	if (statusCode == 304) {
// 		return (generateErrorPage(304));
// 	}

// 	if (method == "GET" || method == "HEAD") {
// 		return (createHandleGet());
// 	} else if (!isDirectory(_uri) && method == "POST") {
// 		return (createHandlePost(clientSocket));
// 	} else if (!isDirectory(_uri) && method == "DELETE") {
// 		return (createHandleDelete());
// 	}

// 	return (generateErrorPage(405));
// 	// return (httpStatusCode(405) + "Content-Type: text/html\r\n\r\n<html><body><h1>Method Not Allowed</h1></body></html>");
// }

/*Creat An Instance of GetHandler Class And
Call The Get Method To Prepare The Response.*/
std::string HTTPResponse::createHandleGet() {
	GetHandler  Get(_requestMap, _serverConfig);
	return (Get.GetMethod());
}

std::string HTTPResponse::createHandlePost(int const clientSocket) {
	// std::string responseBody = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n<html><body><h1>POST Request Received</h1></body></html>";
	// return responseBody;
		this->_post.handlePost(this->_requestString, clientSocket);
		// std::cout << "POST REQUEST RECEIVED =========> " << std::endl
		std::string response = this->_post.getResponses()[clientSocket];
		this->_post.getResponses().erase(clientSocket);
		this->_post.getPostData().clear();
		this->_post.printPostData();
		this->_post.printPostResponses();
		return (response);
}

std::string HTTPResponse::createHandleDelete() {
	std::string responseBody = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n\r\n<html><body><h1>DELETE Request Received</h1></body></html>";
	return responseBody;
}

// std::string httpGeneralHeader

/*Return Message Corresponding To The Status Code Is Passed.*/
std::string HTTPResponse::httpStatusCode(int statusCode) {
	switch (statusCode) {
		case 200: return "HTTP/1.1 200 OK";
		case 400: return "HTTP/1.1 400 Bad Request";
		case 403: return "HTTP/1.1 403 Forbidden";
		case 404: return "HTTP/1.1 404 Not Found";
		case 405: return "HTTP/1.1 405 Method Not Allowed";
		case 304: return "HTTP/1.1 304 Not Modified";
		default:  return "HTTP/1.1 500 Internal Server Error";
	}
}

/*Reading The Binary From The Path and Return a String*/
std::string HTTPResponse::readBinaryFile(std::string const & path) {
    std::ifstream fileStream(path.c_str(), std::ios::binary);
    if (!fileStream.is_open()) {
		Server::logMessage("ERROR: File Not Open In The readBinaryFile Function!");
        perror("error:");
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

static void free_dptr( char** env ) {
	size_t i = 0;
	if (!env)
		return ;
	while (env[i]) {
		free(env[i]);
		env[i] = NULL;
		i++;
	}
	free(env);
	env = NULL;
}

/* CGI EXECUTION */
std::string HTTPResponse::cgi(std::string& uri) {
	std::string path = _serverConfig["root"] + uri;
	std::string responseBody = "";
	char** env = createEnv();

	if (!env) {
		Server::logMessage("ERROR: Environment Variable Not Available!");
		return (generateErrorPage(500));
	}
	int fd_pipe[2];
	if (pipe(fd_pipe) == -1) {
		free_dptr(env);
		Server::logMessage("ERROR: 1st Pipe Creation In The cgi Function Failed!");
		return (generateErrorPage(500));
	}
	pid_t forked_ps = fork();
	if (forked_ps == -1) {
		free_dptr(env);
		Server::logMessage("ERROR: 1st Fork In The cgi Function Failed!");
		return (generateErrorPage(500));
	} else if (forked_ps == 0) { // Child process
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		// Handle POST data
		if (_requestMap["method"] == "POST") {
			int input_fd[2];
			if (pipe(input_fd) == -1) {
				Server::logMessage("ERROR: 2nd Pipe Creation In The cgi Function Failed!");
				exit(1); // Exit if pipe fails
			}
			pid_t input_fork = fork();
			if (forked_ps == -1)
				Server::logMessage("ERROR: 2nd Fork In The cgi Function Failed!");
			if (input_fork == 0) { // Child process to handle stdin
				close(input_fd[0]);
				write(input_fd[1], _requestMap["body"].c_str(), _requestMap["body"].length());
				close(input_fd[1]);
				Server::logMessage("ERROR: 1nd Execv Failed!");
				exit(0);
			} else {
				close(input_fd[1]);
				dup2(input_fd[0], STDIN_FILENO);
				close(input_fd[0]);
			}
		}
		// argv array for execve
		char *argv[] = {strdup(path.c_str()), NULL};
		execve(path.c_str(), argv, env); // Execute the CGI script
		close(fd_pipe[1]);
		free(argv[0]);
		Server::logMessage("ERROR: 2nd Execv Failed!");
		exit(1); // If execve fails, exit the child process
	} else { // Parent process
		close(fd_pipe[1]);
		char buff[100];
		int ret = 1;
		while (ret > 0) {
			bzero(buff, sizeof(buff));
			ret = read(fd_pipe[0], buff, 100 - 1);
			responseBody += buff;
		}
		close(fd_pipe[0]);
		free_dptr(env);

		int status;
		waitpid(forked_ps, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			Server::logMessage("INFO: CGI Passed!");
			return responseBody;
		} else {
			Server::logMessage("ERROR: CGI Failed!");
			return (generateErrorPage(500));
		}
	}
}

char** HTTPResponse::createEnv() {
	std::map<std::string, std::string>::iterator it = _requestMap.begin();
	char** env = (char**)calloc(_requestMap.size() + 5, sizeof(char*)); // Adjust size as needed
	if (!env){
		Server::logMessage("ERROR: Environment Variable Creation Failed!");
		return NULL;
	}
	size_t i = 0;
	while (it != _requestMap.end()) {
		std::string env_var = (*it).first;
		std::transform(env_var.begin(), env_var.end(), env_var.begin(), ::toupper);
		env_var += "=" + (*it).second;
		env[i] = strdup(env_var.c_str());
		if (!env[i]) {
			free_dptr(env);
			return NULL;
		}
		it++;
		i++;
	}
	// Add necessary CGI environment variables
	std::string script_name = "SCRIPT_NAME=" + _requestMap["uri"];
	env[i++] = strdup(script_name.c_str());

	std::string request_method = "REQUEST_METHOD=" + _requestMap["method"];
	env[i++] = strdup(request_method.c_str());

	std::string server_name = "SERVER_NAME=" + _serverConfig["server_name"];
	env[i++] = strdup(server_name.c_str());

	std::string server_protocol = "SERVER_PROTOCOL=HTTP/1.1";
	env[i++] = strdup(server_protocol.c_str());

	if (_requestMap["method"] == "POST") {
		std::stringstream ss;
		ss << "CONTENT_LENGTH=" << _requestMap["body"].size();
		std::string content_length = ss.str();
		env[i++] = strdup(content_length.c_str());
	}

	// Add other environment variables as needed
	env[i] = NULL;
	Server::logMessage("INFO: Environment Variable Created!");
	return env;
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
		_responses[clientSocket] = getResponse(clientSocket);
		Server::logMessage("INFO: Response Generated for socket fd: " + clientSocket);
		return true;
	}
	if (pollEvent == POLLOUT_TMP) {
		std::map<int, std::string>::iterator iter = _responses.find(clientSocket);
		if (iter == _responses.end()) {
			Server::logMessage("Error: No response In The _responses found for socket fd: " + clientSocket);
			// std::cerr << "No response found for socket fd " << clientSocket << std::endl;
			return false;
		}
		std::string response = iter->second;
		// printStringToFile(response, "./src/request/response.txt");
        ssize_t bytesSent = send(clientSocket, this->_responses[clientSocket].c_str(), this->_responses[clientSocket].size(), 0);
        // ssize_t bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
        if (bytesSent == -1) {
			Server::logMessage("Error: No Byte Sent for socket fd: " + clientSocket);
			return false;
        }
		connectedSocket.setConnectionStartTime();
		if (bytesSent < static_cast<ssize_t>(this->_responses[clientSocket].size())) {
			Server::logMessage("WARNING: Sent Byte Less Than The Response for socket fd: " + clientSocket);
			pollFds[i].events = POLLOUT;
			this->_responses[clientSocket].erase(0, bytesSent);
			connectedSocket.setState(WRITING);
		}
		else {
			Server::logMessage("INFO: Response Sent for socket fd: " + clientSocket);
			connectedSocket.setState(DONE);
			pollFds[i].events = POLLIN;
			pollFds[i].revents = 0;
        	_responses.erase(clientSocket);
		}
        return true;
    }
	Server::logMessage("ERROR: Response Function Failed for socket fd: " + clientSocket);
    return false;
}
// HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>

/*Display Corresponding Response To The Fd Is Passed.*/
void HTTPResponse::displayResponse(int fd) {
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

void HTTPResponse::displayRequestMap()
{
	std::cout << RED "****Request Map Inside HTTPResponse:\n";
	std::map<std::string, std::string>::iterator itr = _requestMap.begin();
	for (; itr != _requestMap.end(); itr++)
		std::cout << ORG << itr->first << ":" MAGENTA << itr->second << RESET << std::endl;
}

void HTTPResponse::displayServerConfig()
{
	std::cout << RED "****The server config map Inside HTTPResponse:\n";
	std::map<std::string, std::string>::iterator itr = _serverConfig.begin();
	for (; itr != _serverConfig.end(); itr++)
		std::cout << ORG << itr->first << "->" MAGENTA << itr->second << RESET << std::endl;
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
	Server::logMessage("INFO: Error Header Created, StatusCode: " + statusCode);
    return headers.str();
}

std::string intToString(int const i) {
	std::ostringstream convert;
	convert << i;
	return (convert.str());
}

std::string HTTPResponse::generateDefaultErrorPage(int statusCode, std::string const & message) {
	Server::logMessage("INFO: Default Error Body Dynamically Generated, StatusCode: " + statusCode);
    // Replace with your custom error page logic
    std::string content = "<html>\r\n<head><title>" + intToString(statusCode) + " " + message + 
		"</title></head>\r\n<body>\r\n<center><h2>" + intToString(statusCode) + " "	+ message +
		"</h2></center>\r\n<hr><center>Musketeers Group!</center>\r\n</body>\r\n</html>";
    
	return content;
}


std::string HTTPResponse::generateErrorPage(int statusCode) {
    std::string errorFilePath = "./src/transfer/errors/" + intToString(statusCode) + ".html";

    std::ifstream file(errorFilePath.c_str());
    if (file.is_open()) {
		Server::logMessage("INFO: Default Error Page Statically Read, StatusCode: " + statusCode);
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
