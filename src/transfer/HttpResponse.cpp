/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 00:46:45 by nnavidd           #+#    #+#             */
/*   Updated: 2024/08/07 14:30:30 by nnabaeei         ###   ########.fr       */
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
        return 400;
    }
    return 200;
}

/*Return Corresponding Status Code Response Or In Case
Of Responding With An Specific Method Is Required,
It Invokes Corresponding Method.*/
std::string HTTPResponse::getResponse(int const clientSocket) {
    int statusCode = validate();

    std::string method = _requestMap["method"];
    // std::cout << RED "****received method is: " BLUE << method << RESET << std::endl;
    if (statusCode == 400) {
        return httpStatusCode(400) + "Content-Type: text/html\r\n\r\n<html><body><h1>Bad Request</h1></body></html>";
    }
    if (statusCode == 304) {
        return httpStatusCode(304);
    }

    if (method == "GET" || method == "HEAD") {
        return createHandleGet();
    } else if (method == "POST") {
        return createHandlePost(clientSocket);
    } else if (method == "DELETE") {
        return createHandleDelete();
    }
    return httpStatusCode(405) + "Content-Type: text/html\r\n\r\n<html><body><h1>Method Not Allowed</h1></body></html>";
}

/*Creat An Instance of GetHandler Class And
Call The Get Method To Prepare The Response.*/
std::string HTTPResponse::createHandleGet() {
    GetHandler  Get(_requestMap, _serverConfig);
    return (Get.GetMethod());
}

std::string HTTPResponse::createHandlePost(int const clientSocket) {
	//****************print header map********************
		displayRequestMap();
	//****************************************************
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
        case 200: return "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
        case 400: return "HTTP/1.1 400 Bad Request\r\nConnection: keep-alive\r\n";
        case 404: return "HTTP/1.1 404 Not Found\r\nConnection: keep-alive\r\n";
        case 304: return "HTTP/1.1 304 Not Modified\r\nConnection: keep-alive\r\n";
        case 405: return "HTTP/1.1 405 Method Not Allowed\r\nConnection: keep-alive\r\n";
        default:  return "HTTP/1.1 500 Internal Server Error\r\nConnection: keep-alive\r\n";
    }
}

/*Reading The Binary From The Path and Return a String*/
std::string HTTPResponse::readBinaryFile(std::string const & path) {
    std::ifstream fileStream(path.c_str(), std::ios::binary);
    if (!fileStream.is_open()) {
        perror("error:");
        return "";
    }
    std::ostringstream ss;
    ss << fileStream.rdbuf();
    return ss.str();
}

/*Reading From The Path and Return a String*/
std::string HTTPResponse::readHtmlFile(const std::string &path) {
    std::ifstream fileStream(path.c_str());
    if (!fileStream.is_open()) {
        perror("error:");
        return "";
    }
    std::ostringstream ss;
    ss << fileStream.rdbuf();
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
std::string HTTPResponse::cgi( std::string& uri ) {
	std::cout <<"?? ? ? ? ? ? ? ?" << std::endl;
	std::cout << uri << std::endl;
	std::string path = _serverConfig["root"] + _requestMap["uri"];
	std::string responseBody = "";
	char** env = createEnv(); // what needed for ????

	if (!env) {
		throw Exception("createEnv() failed to allocate", CGI_FAIL);
		return (""); // what do in this case
	}

	int fd_pipe[2];
	int remember_stdin = dup(STDIN_FILENO);
	if (pipe(fd_pipe) == -1) {
		throw Exception("createEnv() failed to fork", CGI_FAIL);
		return (""); // what do in this case
	}
	pid_t forked_ps = fork();
	if (forked_ps == -1) {
		throw Exception("createEnv() failed to fork", CGI_FAIL);
		return (""); // what do in this case
	}
	else if (forked_ps == 0) { // child
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		execve(path.c_str(), NULL, NULL); // make dynamic
		close(fd_pipe[1]);
		throw Exception("createEnv(): command not found", CGI_FAIL);
		return (""); // what do in this case
	}
	else { // parent
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);

		// read from the pipe
		char buff[100];
		int ret = 1;
		while (ret > 0) {
			bzero(buff, sizeof(buff));
			ret = read(fd_pipe[0], buff, 100 - 1);
			responseBody += buff;
		}

		int status;
		waitpid(forked_ps, &status, 0); // probably will block everything ?
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			dup2(remember_stdin, STDIN_FILENO);
			close(fd_pipe[0]);
			free_dptr(env);
			return (responseBody);
		}
	}
	close(fd_pipe[0]);
	free_dptr(env);
	return (""); // error --- the child doesn't execute this, the parent arrives in case the child failed
}

char** HTTPResponse::createEnv( void ) {
	std::map<std::string, std::string>::iterator it = _requestMap.begin();

	char** env = (char**)calloc(_requestMap.size() + 1, sizeof(char*)); // + 1?
	if (!env)
		return (NULL);
	env[_requestMap.size()] = NULL;

	std::cout << "---------------{ cgi env }--------------------" << std::endl;
	size_t i = 0;
	while (it != _requestMap.end()) {
		std::string env_var = (*it).first;
		for (size_t i = 0; i < env_var.length(); i++) {
			env_var[i] = std::toupper(env_var[i]);
		}
		env_var += "=" + (*it).second;
		env[i] = strdup(env_var.c_str());
		if (!env[i]) {
			free_dptr(env);
			return (NULL);
		}
		std::cout << BLUE << "[" << env_var <<  "]" << RESET << std::endl;
		it++;
		i++;
	}
	std::cout << "---------------------------------------------" << std::endl;
	return (env);
}

/*Return a String Of Current Time In a HTTP Header Format.*/
std::string HTTPResponse::formatTimeHTTP(std::time_t rawTime) {
    std::tm *gmTime = std::gmtime(&rawTime);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmTime);
    return std::string(buffer);
}


std::string HTTPResponse::getCurrentTime() {
    std::time_t currentTime = std::time(NULL);
    return formatTimeHTTP(currentTime);
}

/*Generate ETag According To The FilePath Last Modifed And Size.*/
std::string HTTPResponse::generateETag(const std::string &filePath, std::string &date, std::string &lastModified) {
    struct stat fileInfo;

    if (stat(filePath.c_str(), &fileInfo) != 0) {
        std::cerr << "Error getting file information: " << filePath << std::endl;
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
        // std::cout << RED "Handled request on socket fd " RESET << clientSocket << std::endl;
        return true;
    }
    if (pollEvent == POLLOUT_TMP) {
        std::map<int, std::string>::iterator iter = _responses.find(clientSocket);
        if (iter == _responses.end()) {
            std::cerr << "No response found for socket fd " << clientSocket << std::endl;
            return false;
        }
        std::string response = iter->second;

        // Print the provided response in command prompt
        // displayResponse(clientSocket);
        // Print the provided response in file
        printStringToFile(response, "./src/request/response.txt");

        ssize_t bytesSent = send(clientSocket, this->_responses[clientSocket].c_str(), this->_responses[clientSocket].size(), 0);
				std::cout << RED << "bytes sent" << bytesSent << std::endl;
				std::cout << "this->_responses[clientSocket].size()" << this->_responses[clientSocket].size() << RESET << std::endl;
        if (bytesSent == -1) {
            std::cerr << RED << "Sending response failed" << RESET << std::endl;
            return false;
        }

				// std::cout << this->_responses[clientSocket] << std::endl;
				std::cout << "Response handled by connected socket " << clientSocket << std::endl;
				if (bytesSent < static_cast<ssize_t>(this->_responses[clientSocket].size())) {
					pollFds[i].events = POLLOUT;
					this->_responses[clientSocket].erase(0, bytesSent);
					connectedSocket.setState(WRITING);
				}
				else if (connectedSocket.getState() != WRITING) {
					pollFds[i].events = POLLIN;
        _responses.erase(clientSocket);
				}
        return true;
    }
    return false;
}

/*Display Corresponding Response To The Fd Is Passed.*/
void HTTPResponse::displayResponse(int fd) {
    std::cout << ORG "****Response for fd [" << fd << "] is:\n" << MAGENTA << _responses[fd] << RESET << std::endl;
}

/*Print A String In Passed File Path.*/
void HTTPResponse::printStringToFile(const std::string& string, const std::string& path) {
    // std::cout << RED "****Printing response in file: " BLUE << path << RESET << std::endl;
    std::ofstream outfile(path.c_str());
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
	std::cout << RED "****Request Map IN Response:\n";
	std::map<std::string, std::string>::iterator itr = _requestMap.begin();
	for (; itr != _requestMap.end(); itr++)
		std::cout << ORG << itr->first << ":" MAGENTA << itr->second << RESET << std::endl;
}

void HTTPResponse::displayServerConfig()
{
	std::cout << RED "****The server config map IN Response:\n";
	std::map<std::string, std::string>::iterator itr = _serverConfig.begin();
	for (; itr != _serverConfig.end(); itr++)
		std::cout << ORG << itr->first << "->" MAGENTA << itr->second << RESET << std::endl;
}