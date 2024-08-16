/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/15 13:30:53 by nnabaeei         ###   ########.fr       */
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
		GetHandler(const std::map<std::string, std::string>& requestMap, const std::map<std::string, std::string>& serverConfig);
		~GetHandler();
		std::string GetMethod();
	
	private:
	std::string handleDirectoryListing(const std::string& dirPath);
	std::string findIndexFile(std::string const & dirPath);
	std::string const setServerRoot(std::string const & filePath);

	// protected:
	// 	std::string createHandleGet();
};

#endif // GETHANDLER_HPP
