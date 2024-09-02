/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:28:20 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/02 14:39:58 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include <map>
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>
# include "Poll.hpp"
# include "colors.h"
# include "ConnectedSocket.hpp"
# include "HttpResponse.hpp"

class Post : public HTTPResponse {
	public:
		Post(void);
		Post(Post const &other);
		Post &operator=(Post const &rhs);
		~Post(void);
		std::string handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket, std::map<std::string, std::string> &serverConfig);
		
	private:
		bool _isFileSaved;

		void parsePostRequest(std::string const &requestHeader, std::ostringstream const &requestBody);
		std::string getDelimiter(std::string request);
		// std::string getBody(std::string request);
		void getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter);
		void getSubmitedData(std::string &contentDisposition);
		std::string getName(std::string string);
		std::string getFileName(std::string string);
		void saveFile(std::string string);
		ChildProcessData handlePostCgi(ConnectedSocket &connectedSocket);
		std::string findCommand(std::string const &command);
		void cgiError(ConnectedSocket &connectedSocket);
		void handleCgiChildProcess(ConnectedSocket &connectedSocket, int pipeFds[2]);
		void handleCgiMainProcess(ConnectedSocket &connectedSocket, int pipeFds[2], pid_t id);
		void UpdateCgiProperties(ConnectedSocket &connectedSocket, pid_t id, int pipeFds[2], bool isError);
};



#endif
