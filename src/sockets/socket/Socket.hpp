/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:25:55 by fahmadia          #+#    #+#             */
/*   Updated: 2024/09/10 09:20:17 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <stdexcept>
# include <string>
# include <unistd.h>
# include "errors.h"
# include "../../exception/Exception.hpp"

class Socket {
	private:
		int _socketFd;
		
	public:
		Socket(void);
		Socket(Socket const &other);
		Socket(int socketFd);
		Socket &operator=(Socket const &rhs);
		virtual ~Socket(void);

		int getSocketFd(void) const;
		void setSocketFd(int socketFd);
};

#endif
