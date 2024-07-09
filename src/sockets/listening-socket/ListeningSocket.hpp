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
# include "Socket.hpp"
# include "ConnectedSocket.hpp"

class HttpServer;

class ListeningSocket : public Socket {
	private:
		unsigned int _maxIncomingConnections;
		std::string _ip;
		std::string _port;
		addrinfo *_addressInfo;



	public:
		ListeningSocket(void);
		ListeningSocket(unsigned int maxIncomingConnections, std::string const &ip, std::string const &port);
		ListeningSocket(ListeningSocket const &other);
		ListeningSocket &operator=(ListeningSocket const &rhs);
		~ListeningSocket(void);

		struct addrinfo *allocateAddrInfo(void) const;
		int createSocket(void) const;
		addrinfo *getAddressInfo(void) const;
		std::string const &getIp(void) const;
		std::string const &getPort(void) const;
		unsigned int getMaxIncomingConnections(void) const;
		void setAddressInfo(addrinfo *addressInfo);
};

#endif
