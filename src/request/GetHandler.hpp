/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnavidd <nnavidd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/30 14:49:54 by nnavidd          ###   ########.fr       */
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
