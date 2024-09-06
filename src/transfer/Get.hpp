/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/09/06 19:20:43 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include "HttpResponse.hpp"
# include <iostream>
# include <string>
# include <dirent.h>
# include <sys/stat.h>
# include <vector>
# include <algorithm>
# include <fstream>
# include "Server.hpp"

class Get : public HTTPResponse {
	public:
		Get(const std::map<std::string, std::string>& requestMap, const std::map<std::string, std::string>& serverConfig, std::vector<LocationConf> const &locations);
		~Get();
		std::string handleGet(ConnectedSocket &connectedSocket);
	
	private:
	std::map<std::string, std::string> _redirections;
	
	std::string handleDirectoryListing(const std::string& dirPath);
	std::string findIndexFile(std::string const & dirPath);
	std::string const setServerRoot(std::string const & filePath);
	std::string getRedirection(std::string const &uri);
	void setRedirections(void);
	void printRedirections(void);

	// protected:
	// 	std::string createHandleGet();
};

#endif // GETHANDLER_HPP
