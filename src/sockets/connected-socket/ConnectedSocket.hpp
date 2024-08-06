/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:13 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/05 20:03:00 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTED_SOCKET_HPP
# define CONNECTED_SOCKET_HPP

# include <netdb.h>
# include <ctime>
# include <iostream>
# include "../socket/Socket.hpp"

class ConnectedSocket : public Socket {
	private:
		struct sockaddr_storage _incomingRequestAddress;
		socklen_t _incomingConnectionAddressSize;
		bool _isConnected;
	public:
		int _iterationNum;
	private:
		time_t _connectionStartTime;

	public:
		ConnectedSocket(void);
		ConnectedSocket(int socketFd, sockaddr_storage const &incomingRequestAddress, socklen_t  const &incomingConnectionAddressSize);
		ConnectedSocket(ConnectedSocket const &other);
		ConnectedSocket &operator=(ConnectedSocket const &rhs);
		~ConnectedSocket(void);

		sockaddr_storage const &getIncomingRequestAddress(void) const;
		socklen_t const &getIncomingConnectionAddressSize(void) const;
		bool getIsConnected(void) const;
		time_t const &getConnectionStartTime(void) const;
		void setConnectionStartTime();

		void setIsConnected(bool isConnected);
};

#endif
