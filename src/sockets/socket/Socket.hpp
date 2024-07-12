/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:25:55 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/12 11:38:31 by ncasteln         ###   ########.fr       */
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
		Socket &operator=(Socket const &rhs);	// unused
		virtual ~Socket(void);

		int getSocketFd(void) const;
		void setSocketFd(int socketFd);
};

#endif
