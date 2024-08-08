/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:13 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/07 17:44:27 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTED_SOCKET_HPP
# define CONNECTED_SOCKET_HPP

# include <netdb.h>
# include <ctime>
# include <iostream>
# include "../socket/Socket.hpp"

typedef enum e_state {
	CREATED = 1,
	READING,
	WRITING,
	KEEP_ALIVE,
	DONE,
	CLOSED
}t_state;

class ConnectedSocket : public Socket {
	private:
		struct sockaddr_storage _incomingRequestAddress;
		socklen_t _incomingConnectionAddressSize;
		bool _isConnected;
		int _iterationNum;
		time_t _connectionStartTime;
		t_state _state;

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
		int getIterationNum();
		t_state getState(void);
		void setState(t_state state);
		void setConnectionStartTime();
		void setIterationNum(int iterationNum);

		void setIsConnected(bool isConnected);
};

#endif
