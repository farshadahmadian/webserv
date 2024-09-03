/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/09/03 09:03:38 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETHANDLER_HPP
# define GETHANDLER_HPP

# include "HttpResponse.hpp"
# include <iostream>
# include <string>
# include <dirent.h>
# include <sys/stat.h>
# include <vector>
# include <algorithm>
# include <fstream>
# include "Server.hpp"

class GetHandler : public HTTPResponse {
	public:
		GetHandler(const std::map<std::string, std::string>& requestMap, const std::map<std::string, std::string>& serverConfig, std::vector<LocationConf> const &locations);
		~GetHandler();
		std::string GetMethod(ConnectedSocket &connectedSocket);
	
	private:
	std::map<std::string, std::string> _redirections;
	
	std::string handleDirectoryListing(const std::string& dirPath);
	std::string findIndexFile(std::string const & dirPath);
	std::string const setServerRoot(std::string const & filePath);
	std::string getRedirections(std::string const &uri);
	void setRedirections(void);
	void printRedirections(void);

	// protected:
	// 	std::string createHandleGet();
};

#endif // GETHANDLER_HPP
