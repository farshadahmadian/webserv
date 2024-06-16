/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:24:23 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/16 15:02:31 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Stream_SOCKET_HPP
# define Stream_SOCKET_HPP

# include <string>
# include <netdb.h>
# include <cstring>
# include <iostream>
# include "colors.h"
# include "error.hpp"

class StreamSocket {
	public:
		StreamSocket(void);

		class SocketException;

		

	private:
		struct addrinfo *initAddrInfo(void);
		int createSocket(void);
		void setPortAvailable(void);
		void bindSocket(void);

		std::string const _port;
		std::string const _ip;
		
		addrinfo *_addrInfo;
		int _socketFd;
};

class StreamSocket::SocketException : public std::exception {
	public:
		SocketException(void);
		SocketException(std::string errMsg);
		~SocketException(void) throw();

		char const *what() const throw();

		t_error getError(void) const;
		void setError(t_error error);
	
	private:
		std::string _errMsg;
		t_error _error;
};

#endif