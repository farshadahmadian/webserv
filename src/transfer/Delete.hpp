/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:02:42 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/14 16:23:24 by fahmadia         ###   ########.fr       */
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
# include <cstring>
# include "ConnectedSocket.hpp"
# include "HttpResponse.hpp"

class Delete : public HTTPResponse {

	private:
		Delete &operator=(Delete const &rhs);

	public:
		Delete(void);
		~Delete(void);
		Delete(Delete const &other);

		void handleDelete(ConnectedSocket &connectedSocket);
		void parseDeleteRequest(std::string const &requestHeader, std::ostringstream const &requestBody);
		std::string getDelimiter(std::string request);
		void getSubmittedFormInputs(std::string body, std::string formFieldsDelimiter);
		void getSubmitedData(std::string &contentDisposition);
		std::string getFileName(std::string string);
		bool deleteFile(ConnectedSocket &connectedSocket);
};

#endif
