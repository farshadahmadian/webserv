/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:28:20 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/27 17:05:23 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include <map>
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include "colors.h"

class Post {
	public:
		Post(void);
		Post(Post const &other);
		Post &operator=(Post const &rhs);
		~Post(void);

		std::map<int, std::string> &getResponses(void);

		void handlePost(std::string request, int connectedSocketFd);
	private:
		std::map<int, std::string> _responses;
		std::map<std::string, std::string> _postData;

		void parsePostRequest(std::string request);
		std::string getDelimiter(std::string request);
		std::string getBody(std::string request);
		void getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter);
		void getSubmitedData(std::string &contentDisposition);
		std::string getName(std::string string);
		std::string getFileName(std::string string);
		void saveFile(std::string string);

		void printPostData(void);
};

#endif
