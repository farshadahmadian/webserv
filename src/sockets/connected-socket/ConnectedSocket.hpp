/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:13 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 16:24:52 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTED_SOCKET_HPP
# define CONNECTED_SOCKET_HPP

# include <netdb.h>
# include "../socket/Socket.hpp"

class ConnectedSocket : public Socket {
	private:
		struct sockaddr_storage _incomingRequestAddress;
		socklen_t _incomingConnectionAddressSize;

	public:
		ConnectedSocket(void);
		ConnectedSocket(int socketFd, sockaddr_storage const &incomingRequestAddress, socklen_t  const &incomingConnectionAddressSize);
		ConnectedSocket(ConnectedSocket const &other);
		ConnectedSocket &operator=(ConnectedSocket const &rhs);
		~ConnectedSocket(void);

		sockaddr_storage const &getIncomingRequestAddress(void) const;
		socklen_t const &getIncomingConnectionAddressSize(void) const;
};

#endif
