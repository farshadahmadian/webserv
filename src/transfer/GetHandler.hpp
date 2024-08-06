/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/08/03 09:39:37 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETHANDLER_HPP
# define GETHANDLER_HPP

# include "HttpResponse.hpp"

class GetHandler : public HTTPResponse {
	public:
		GetHandler(const std::map<std::string, std::string>& requestMap, const std::map<std::string, std::string>& serverConfig);
		~GetHandler();
		std::string GetMethod();
	
	// protected:
	// 	std::string createHandleGet();
};

#endif // GETHANDLER_HPP
