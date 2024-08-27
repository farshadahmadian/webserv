/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:46:45 by nnavidd           #+#    #+#             */
/*   Updated: 2024/08/27 17:36:53 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "GetHandler.hpp"
#include "Exception.hpp"
#include "Post.hpp"
#include "Delete.hpp"

HTTPResponse::HTTPResponse() : _storageDirectory("./www/farshad/cloudStorage"), _data(std::map<std::string, std::string>()) {
	// std::cout << CYAN "HTTPResponse constructor called\n" RESET;
}

HTTPResponse::HTTPResponse(std::map<std::string, std::string> const & serverConfig) :
	_serverConfig(serverConfig)  {
	loadMimeTypes(MIME);
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

bool HTTPResponse::isDirectory(const std::string& uri) const {
	std::string filePath = _serverConfig.at("root") + uri;
	std::cout << "Hiii: " << filePath << std::endl;
	struct stat st;
	if (stat(filePath.c_str(), &st) != 0) {
		return false; // Error in accessing the path or path does not exist
	}
	return S_ISDIR(st.st_mode);
}

/*Return Corresponding Status Code Response Or In Case
Of Responding With An Specific Method Is Required,
It Invokes Corresponding Method.*/
std::string HTTPResponse::getResponse(int const clientSocket, ConnectedSocket &connectedSocket) {
	int statusCode = validate();

	std::string method = _requestMap["method"];
	std::string uri = _requestMap["uri"];

	displayRequestMap();
	displayServerConfig();
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
	if ((method == "POST" || method == "DELETE")) {// && !isDirectory(uri)) {
		if (method == "POST" && this->_requestMap["uri"] == "/delete")
			return createHandleDelete(connectedSocket);
		else if (method == "POST")
			return createHandlePost(clientSocket, connectedSocket);
		else if (method == "DELETE")
			return createHandleDelete(connectedSocket);
	}

	// If none of the above conditions are met, return a 405 Method Not Allowed error
	return generateErrorPage(405);
}


/*Creat An Instance of GetHandler Class And
Call The Get Method To Prepare The Response.*/
std::string HTTPResponse::createHandleGet() {
	GetHandler  Get(_requestMap, _serverConfig);
	return (Get.GetMethod());
}

std::string HTTPResponse::createHandlePost(int const connectedSocketFd, ConnectedSocket &connectedSocket) {
	Post postResponse;
	postResponse.handlePost(connectedSocketFd, connectedSocket);
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
		case 400: return "HTTP/1.1 400 Bad Request";
		case 403: return "HTTP/1.1 403 Forbidden";
		case 404: return "HTTP/1.1 404 Not Found";
		case 405: return "HTTP/1.1 405 Method Not Allowed";
		case 413: return "HTTP/1.1 413 Payload Too Large";
		case 503: return "HTTP/1.1 503 Service Unavailable";
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

/*Check whether the accepted cgi extension exits or not.*/
bool HTTPResponse::isCGI(std::string const & filePath) {
    size_t pos = acceptedCgiExtention(filePath);
    // if (pos != std::string::npos && pos + 3 < filePath.length()) {
		if (pos != std::string::npos) {
        // char charAfterExtension = filePath[pos + 3];
        // if (charAfterExtension == '/' || charAfterExtension == '\0' || charAfterExtension == '?') {
            return true;
        }
    // }
    return false;
}

/*create body of the received cgi response.*/
std::string const HTTPResponse::handleCGI(std::string & uri) {
	std::string cgiResult = cgi(uri);

	std::string content = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n"
	"<meta charset=\"UTF-8\">\r\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
	"<title>CGI Execution Result</title>\r\n<style>\r\nbody {font-family: Arial, sans-serif; margin: 20px;}\r\n"
	"pre { background-color: #f4f4f4; padding: 10px; border: 1px solid #ddd; overflow-x: auto;}\r\n</style>\r\n"
	"</head>\r\n<body>\r\n<h1>CGI Execution Result</h1>\r\n<pre><code>\r\n"
	+ cgiResult + "\r\n</code></pre>\r\n</body>\r\n</html>";
	
	return (content);
}

/*check the cgi extension, and return the corresponding interpreter.*/
std::string const setInterpreter(std::string const & cgiPath) {
	if (cgiPath.substr(cgiPath.find_last_of(".")) == ".pl") {
		return("/usr/bin/perl");
	} else if (cgiPath.substr(cgiPath.find_last_of(".")) == ".py") {
		return("/usr/bin/python3");
	} else {
		// Handle other cases, or default to shell script execution
		return("/bin/bash"); // Assuming the default is shell scripts
	}
}

bool createPipes(int fd_pipe[2]) {
	if (pipe(fd_pipe) == -1) {
		Server::logMessage("ERROR: Pipe creation failed!");
		return false;
	}
	return true;
}

void executeCGI(const std::string& path, char** env, const std::string& method, const std::string& body, int fd_pipe[2]) {
	close(fd_pipe[0]);
	dup2(fd_pipe[1], STDOUT_FILENO);

	if (method == "POST") {
		int input_fd[2];
		if (pipe(input_fd) == -1) {
			Server::logMessage("ERROR: Input pipe creation failed!");
			exit(1);
		}
		pid_t input_fork = fork();
		if (input_fork == 0) {
			close(input_fd[0]);
			write(input_fd[1], body.c_str(), body.length());
			close(input_fd[1]);
			exit(0);
		} else {
			close(input_fd[1]);
			dup2(input_fd[0], STDIN_FILENO);
			close(input_fd[0]);
		}
	}

	std::string interpreter = setInterpreter(path);
	std::cout << "<body><h2><center>Musketeers Group!</center></h2><hr></body></html>"<<std::endl; 

	char *argv[] = {strdup(interpreter.c_str()), strdup(path.c_str()), NULL};
	execve(interpreter.c_str(), argv, env);

	Server::logMessage("ERROR: execve failed!");
	close(fd_pipe[1]);
	free(argv[0]);
	exit(1);
}

std::string HTTPResponse::readFromCGI(int fd_pipe[2], pid_t forked_ps, char** env, int timeout) {
	std::string responseBody;
	close(fd_pipe[1]);

	char buff[100];
	int ret = 1;
	fd_set readfds;
	struct timeval tv;
	int fd_max = fd_pipe[0];
	time_t start_time = time(NULL);

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(fd_pipe[0], &readfds);

		tv.tv_sec = timeout - (time(NULL) - start_time);
		tv.tv_usec = 0;

		if (tv.tv_sec <= 0) {
			Server::logMessage("ERROR: CGI Timeout!");
			kill(forked_ps, SIGKILL);
			waitpid(forked_ps, NULL, 0);
			close(fd_pipe[0]);
			free_dptr(env);
			return generateErrorPage(504);
		}

		int sel = select(fd_max + 1, &readfds, NULL, NULL, &tv);
		if (sel > 0) {
			if (FD_ISSET(fd_pipe[0], &readfds)) {
				bzero(buff, sizeof(buff));
				ret = read(fd_pipe[0], buff, sizeof(buff) - 1);
				if (ret > 0) {
					responseBody += buff;
				} else if (ret == 0) {
					break;
				} else {
					Server::logMessage("ERROR: Reading from CGI failed!");
					close(fd_pipe[0]);
					free_dptr(env);
					return generateErrorPage(500);
				}
			}
		} else if (sel == 0) {
			Server::logMessage("ERROR: CGI Timeout!");
			kill(forked_ps, SIGKILL);
			waitpid(forked_ps, NULL, 0);
			close(fd_pipe[0]);
			free_dptr(env);
			return generateErrorPage(504);
		} else {
			Server::logMessage("ERROR: select() failed!");
			close(fd_pipe[0]);
			free_dptr(env);
			return generateErrorPage(500);
		}
	}

	close(fd_pipe[0]);
	free_dptr(env);
	return responseBody;
}

/*Check and retrieve the status code inside the error generated inside the readFromCGI.*/
bool checkStatusCode(std::string & text, int *err = NULL) {
	std::string code = text.substr(text.find("HTTP/1.1 ") + 9, 3);
	if (code[0] == '5') {
		if (err != NULL)
			*err = Server::stringToInt(code);
		return true;
	}
	return false;
}

std::string HTTPResponse::cgi(std::string& uri) {
	char** env = createEnv(&uri);    
	std::string path = _serverConfig["root"] + uri;

	if (!env) {
		Server::logMessage("ERROR: Environment Variable Not Available!");
		return generateErrorPage(500);
	}

	int fd_pipe[2];
	if (!createPipes(fd_pipe)) {
		free_dptr(env);
		return generateErrorPage(500);
	}

	pid_t forked_ps = fork();
	if (forked_ps == -1) {
		free_dptr(env);
		Server::logMessage("ERROR: Fork failed!");
		return generateErrorPage(500);
	} else if (forked_ps == 0) {
		executeCGI(path, env, _requestMap["method"], _requestMap["body"], fd_pipe);
	} else {
		std::string responseBody = readFromCGI(fd_pipe, forked_ps, env, 5);
		int status;
		waitpid(forked_ps, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			Server::logMessage("INFO: CGI Passed!");
			return responseBody;
		} else if (!responseBody.empty() && checkStatusCode(responseBody)) {
			int err;
			checkStatusCode(responseBody, &err);
			Server::logMessage("ERROR: CGI Failed!");
			return generateErrorPage(err);
		}
	}
	return generateErrorPage(500);
}

/*Check whether the accepted script format exists, returns its position,
otherwise returns npos*/
size_t HTTPResponse::acceptedCgiExtention(std::string const &filePath) {
	std::vector<std::string> cgiExtension;
	size_t	pos;
	size_t	extensionPos;
	int		count = 0;
	cgiExtension.push_back(".sh");
	cgiExtension.push_back(".pl");
	cgiExtension.push_back(".py");
	for (std::vector<std::string>::iterator it = cgiExtension.begin(); it != cgiExtension.end(); ++it) {
		pos = filePath.find(*it);
		if (pos != std::string::npos)
		{
			extensionPos = pos;
			count++;
		}
	}
	if (count == 1)
		return (extensionPos);
	return (std::string::npos);
}

void parseQueryString(std::map<std::string, std::string>& additionalEnvVariable) {
    std::string query;

    // Retrieve the query string either from the URL or the POST body
    if (!additionalEnvVariable["QUERY_STRING"].empty()) {
        query = additionalEnvVariable["QUERY_STRING"];
    } else if (additionalEnvVariable["REQUEST_METHOD"] == "POST" && !additionalEnvVariable["BODY"].empty()) {
        query = additionalEnvVariable["BODY"];
    }

    if (!query.empty()) {
        size_t pos = 0;
        while ((pos = query.find('&')) != std::string::npos) {
            std::string param = query.substr(0, pos);
            size_t eq_pos = param.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = param.substr(0, eq_pos);
                std::string value = param.substr(eq_pos + 1);
                additionalEnvVariable[key] = value;
            }
            query.erase(0, pos + 1);
        }
        // Handle the last key-value pair (or the only one if there's no '&')
        size_t eq_pos = query.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = query.substr(0, eq_pos);
            std::string value = query.substr(eq_pos + 1);
            additionalEnvVariable[key] = value;
        }
    }
}

std::string CutQueryString (std::string * uri, size_t extension, std::string * pathInfo) {
	std::string queryString;
	size_t queryStringPos = uri->find('?');
	if (queryStringPos != std::string::npos) {
		queryString = uri->substr(uri->find('?') + 1);
		*pathInfo = "";
		if (queryStringPos > extension + 3)
			*pathInfo = uri->substr(extension + 3, (queryStringPos - (extension + 3)));
		return (queryString);
	} else if (queryStringPos == std::string::npos)
		queryString = "";
	*pathInfo = uri->substr(extension + 3);
	return (queryString);
}

std::map<std::string, std::string> HTTPResponse::addAdditionalEnvVariables(std::string * uri) {
	std::map<std::string, std::string> additionalEnvVariable;
	std::string pathInfo;
	size_t extension = acceptedCgiExtention(*uri);
	std::string queryString = CutQueryString(uri, extension, &pathInfo);
	*uri = uri->substr(0,extension + 3);

	// Add necessary CGI environment variables
	additionalEnvVariable["PATH_INFO"] = pathInfo;
	additionalEnvVariable["SCRIPT_NAME"] = *uri;
	additionalEnvVariable["REQUEST_METHOD"] = _requestMap["method"];
	additionalEnvVariable["SERVER_NAME"] = _serverConfig["server_name"];
	additionalEnvVariable["SERVER_PROTOCOL"] = _requestMap["version"];
	additionalEnvVariable["SERVER_PORT"] = _serverConfig["port"];
	additionalEnvVariable["HOST_NAME"] = _requestMap["Host"];
	additionalEnvVariable["REQUEST_METHOD"] = _requestMap["method"];
	if (_requestMap["method"] == "POST")
		additionalEnvVariable["CONTENT_LENGTH"] = Server::intToString(_requestMap["body"].size());
	additionalEnvVariable["QUERY_STRING"] = queryString;
	
	parseQueryString(additionalEnvVariable);
	return (additionalEnvVariable);
}

void feedEnv(char **env, std::map<std::string, std::string> & variableMap) {
	size_t i = 0;

	while (env[i] != NULL)
		i++;
	for (std::map<std::string, std::string>::iterator it = variableMap.begin(); it != variableMap.end(); ++it, ++i) {
		std::string env_var = it->first;
		std::transform(env_var.begin(), env_var.end(), env_var.begin(), ::toupper);
		env_var += "=" + it->second;
		env[i] = strdup(env_var.c_str());
		if (!env[i]) {
			free_dptr(env);
			return;
		}
    }
    env[i] = NULL; // Null-terminate the env array
}

/*Retrieve the position of script extension by calling acceptedCgiExtention(),
create a sub string if the infoPath exists,
create the env according to the variable are needed, and return it.*/
char** HTTPResponse::createEnv(std::string * uri) {
	std::map<std::string, std::string> additionalEnvVariables = addAdditionalEnvVariables(uri);
	
	char** env = (char**)calloc(_requestMap.size() + additionalEnvVariables.size() + 1, sizeof(char*)); // Adjust size as needed
	if (!env){
		Server::logMessage("ERROR: Environment Variable Creation Failed!");
		return NULL;
	}
	feedEnv(env, _requestMap);
	feedEnv(env, additionalEnvVariables);
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

		std::cout << CYAN << "Connected socket " << clientSocket << " is sending the response ..." << RESET << std::endl;

		ssize_t bytesSent = send(clientSocket, this->_responses[clientSocket].c_str(), this->_responses[clientSocket].size(), 0);
		if (bytesSent == -1) {
			Server::logMessage("Error: No Byte Sent for socket fd: " + Server::intToString(clientSocket));
			return false;
		}
		std::cout << CYAN << "Bytes sent: " << bytesSent << RESET << std::endl;
		std::cout << CYAN << "Response size: " << this->_responses[clientSocket].size() << RESET << std::endl;

		connectedSocket.setConnectionStartTime();
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

void HTTPResponse::printResponses(void) {
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
