/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/19 15:03:20 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <string>
# include <netdb.h>
# include <stdexcept>
# include "Exception.hpp"
# include "ListeningSocket.hpp"

class HttpServer {
	private:
		std::string const _port;
		std::string const _ip;
		addrinfo *_addressInfo;
		ListeningSocket _listeningSocket;

		struct addrinfo *initAddrInfo(void);
		int createSocket(void);
		
	public:
		HttpServer(void);
		~HttpServer(void);

		ListeningSocket const &getListeningSocket(void) const;
};


#endif
