/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:10:43 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 16:44:56 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <string>
# include <netdb.h>
# include <stdexcept>
# include "../exception/Exception.hpp"
# include "../sockets/listening-socket/ListeningSocket.hpp"
# include "../server/Server.hpp"

class HttpServer {
	private:
		ListeningSocket _listeningSocket;


	public:
		HttpServer(void);
		HttpServer(int maxIncomingConnections, std::string const &ip, std::string const &port);
		HttpServer(HttpServer const &other);
		HttpServer &operator=(HttpServer const &rhs);
		~HttpServer(void);

		ListeningSocket const &getListeningSocket(void) const;
		
		void runServer(void);
};


#endif
