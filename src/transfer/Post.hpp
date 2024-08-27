/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:28:20 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/27 14:51:47 by nnabaeei         ###   ########.fr       */
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
# include "colors.h"
# include "ConnectedSocket.hpp"
# include "HttpResponse.hpp"

class Post : public HTTPResponse {
	public:
		Post(void);
		Post(Post const &other);
		Post &operator=(Post const &rhs);
		~Post(void);
		std::string const & handlePost(int connectedSocketFd, ConnectedSocket &connectedSocket);
		
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

};

#endif
