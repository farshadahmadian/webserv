/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:02:42 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/13 12:16:06 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP

# include <string>
# include <map>
# include <sstream>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <unistd.h>
# include "ConnectedSocket.hpp"

class Delete {
	private:
		std::string _storageDirectory;
		std::map<int, std::string> _responses;
		std::map<std::string, std::string> _deleteData;

	Delete &operator=(Delete const &rhs);
	public:
		Delete(void);
		~Delete(void);
		Delete(Delete const &other);

		std::string const &getSocketResponse(int connectedSocketFd);
		void removeSocketResponse(int connectedSocketFd);
		void clearDeleteData(void);
		std::string getSubStringFromMiddleToIndex(std::string &string, std::string const &toFind, size_t startOffset, size_t endIndex);
		std::string getSubStringFromStartToIndex(std::string &string, std::string const &toFind);
		void handleDelete(ConnectedSocket &connectedSocket);
		void parseDeleteRequest(std::string const &requestHeader, std::ostringstream const &requestBody);
		std::string getDelimiter(std::string request);
		void getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter);
		void getSubmitedData(std::string &contentDisposition);
		std::string getFileName(std::string string);
		bool deleteFile(ConnectedSocket &connectedSocket);
};

#endif
