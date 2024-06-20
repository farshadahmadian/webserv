/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:25:55 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/20 19:52:16 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <stdexcept>
# include <string>
# include <unistd.h>
# include "error.hpp"
# include "Exception.hpp"

class Socket {
	private:
		int _socketFd;
		
	public:
		Socket(Socket const &other);
		Socket(void);
		Socket(int socketFd);
		Socket &operator=(Socket const &rhs);
		virtual ~Socket(void);

		int getSocketFd(void) const;
		void setSocketFd(int socketFd);
};

#endif
