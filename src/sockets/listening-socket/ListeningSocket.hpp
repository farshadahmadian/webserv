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
	private:
		int const _maxIncomingConnections;
		addrinfo *_addressInfo;

		ListeningSocket(void);

	public:
		ListeningSocket(int maxIncomingConnections);
		ListeningSocket(ListeningSocket const &other);
		~ListeningSocket(void);

		addrinfo *getAddressInfo(void) const;
		void setAddressInfo(addrinfo *addressInfo);

		void setPortAvailable(void) const;
		void bindSocket(void) const;
		void listenToRequests(void) const;
		ListeningSocket acceptFirstRequestInQueue(void) const;
};

#endif
