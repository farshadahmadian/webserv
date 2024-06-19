/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:13 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/18 13:28:07 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTED_SOCKET_HPP
# define CONNECTED_SOCKET_HPP

# include "Socket.hpp"

class ConnectedSocket : public Socket {
	private:
		int _socketFd;
		// addr_storage

	public:
		ConnectedSocket(void);
		~ConnectedSocket(void);

};

#endif
