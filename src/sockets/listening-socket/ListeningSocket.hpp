/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:24:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/18 09:57:56 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENING_SOCKET_HPP
# define LISTENING_SOCKET_HPP

# include <string>
# include <netdb.h>
# include <cstring>
# include <iostream>
# include <unistd.h>
# include "colors.h"
# include "error.hpp"
# include "Socket.hpp"

class ListeningSocket : public Socket {
	public:
		ListeningSocket(void);
		~ListeningSocket(void);

		// class SocketException;

		void setPortAvailable(void);
		void bindSocket(void);
		void listenToRequests(void);
		ListeningSocket acceptFirstRequestInQueue(void);
		

	private:
		struct addrinfo *initAddrInfo(void);
		int createSocket(void);

		std::string const _port;
		std::string const _ip;
		int const _maxIncomingConnections;
		
		addrinfo *_addrInfo;
		int _socketFd;
};

// class ListeningSocket::SocketException : public std::exception {
// 	public:
// 		SocketException(void);
// 		SocketException(std::string errMsg);
// 		~SocketException(void) throw();

// 		char const *what() const throw();

// 		t_error getError(void) const;
// 		void setError(t_error error);
	
// 	private:
// 		std::string _errMsg;
// 		t_error _error;
// };

#endif