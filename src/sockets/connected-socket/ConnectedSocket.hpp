/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectedSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:55:13 by fahmadia          #+#    #+#             */
/*   Updated: 2024/08/12 13:52:12 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTED_SOCKET_HPP
# define CONNECTED_SOCKET_HPP

# include <netdb.h>
# include <ctime>
# include <string>
# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include "../socket/Socket.hpp"
# include "colors.h"

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

		std::string _request;
		size_t _contentLength;
		std::ostringstream _requestBody;
		std::string _requestHeader;
		bool _isHeaderComplete;
		std::map<std::string, std::string> _requestMap;
		bool _avoidBodyFirstChunckRepeat;

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
		std::string const &getRequest(void)const;
		size_t getContentLength(void);
		std::ostringstream const &getRequestBody(void) const;
		std::string const &getRequestHeader(void) const;
		bool getIsHeaderComplete(void);
		std::map<std::string, std::string> &getRequestMap(void);
		bool getAvoidBodyFirstChunckRepeat(void);

		void setState(t_state state);
		void setConnectionStartTime();
		void setIterationNum(int iterationNum);
		void setRequestBodyLength(std::string contentLength);
		void setIsHeaderComplete(bool isHeaderComplete);
		void setRequestMap(std::map<std::string, std::string> const &requestMap);
		void setAvoidBodyFirstChunckRepeat(bool isBodyFirstChunckReceived);

		std::string const &appendToRequest(std::string const &toAppend);
		std::ostringstream const & appendToBody(std::ostringstream const &outputStringStream);
		std::string const &appendToHeader(std::string const &toAppend);
		void clearRequestProperties(void);

		void setIsConnected(bool isConnected);
};

#endif
