/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:25:55 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/18 11:48:53 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <stdexcept>
# include <string>
# include <unistd.h>
# include "error.hpp"

class Socket {
	public:
		Socket(void);
		virtual ~Socket(void);

		class SocketException;
};

class Socket::SocketException : public std::exception {
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
