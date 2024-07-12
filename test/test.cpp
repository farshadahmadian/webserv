/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 11:38:09 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/16 10:50:10 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

bool createServer(void) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	// hints.ai_family = AF_INET; // IPv4
	// hints.ai_family = AF_INET6; // IPv6
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	std::string const host = "127.0.0.1";
	std::string const port = "1024"; // port 80 => bind() returns -1 => permission denied (port number must be equal to or greater than 1024 because the port numbers less than 1024 are reserved and are only accessible by root or sudo user)
	struct addrinfo *res;
	int getAddressResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &res); // "res" will be the result which is an allocated linked list (must be deleted)
	if (getAddressResult != 0)
	{
		gai_strerror(getAddressResult);
		// set error
		return false;
	}
	struct addrinfo *temp;

	for (temp = res; temp != NULL; temp = res->ai_next) {
		std::cout << "ip address in network byte order = " << ((sockaddr_in *)temp->ai_addr)->sin_addr.s_addr << std::endl;
		std::cout << "ip address in host byte order = " << inet_ntoa(((sockaddr_in *)temp->ai_addr)->sin_addr) << std::endl;

		std::cout << "port number in network byte order = " << ((sockaddr_in *)temp->ai_addr)->sin_port << std::endl;
		std::cout << "port number in host byte order = " << ntohs(((sockaddr_in *)temp->ai_addr)->sin_port) << std::endl;

	

		std::cout << "Family Type Macro = " << temp->ai_family << std::endl;
		std::cout << "Family Type String = " << getFamilyType(res) << std::endl;
		std::cout << "Protocol Type Macro = " << temp->ai_protocol << std::endl;
		std::cout << "Protocol Type String = " << getProtocolType(res) << std::endl;
		std::cout << "Socket Type Macro = " << temp->ai_socktype << std::endl;
		std::cout << "Socke Type String = " << getSocketType(res) << std::endl;
		std::cout << "size of the struct = " << temp->ai_addrlen << std::endl;
		std::cout << "size of the pointer = " << sizeof(temp->ai_addr) << std::endl;
	}

	int const mainServerSocketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (mainServerSocketFd == -1)
	{
		strerror(errno);
		// set error
		std::cout << "socket failed\n";
		return false;
	}

	
	// if server listening to request on a port stops running, the port will be in TIME-WAIT status for a while (netstat -an | grep TIME_WAIT) to prevent from a new connection being established on that port (because maybe some packets are not arrived to the server, and if the server listens again to that port, the packets from previous connection might be received in the new connection). It makes the port to be unusable for several seconds, to avoid this, we can use setsockopt()
	int reuseAddress = 1;
	int setSocketOptionResult = setsockopt(mainServerSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress));
	if (setSocketOptionResult == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return false;
	}

	// "res->ai_addrlen" is the size of the struct to which "res->ai_addr" points (sizeof(res->ai_addr) is wrong because it returns the size of the pointer itself (8 bytes), and not the size of the struct to which the pointer points)
	int const bindResult = bind(mainServerSocketFd, res->ai_addr, res->ai_addrlen);
	if (bindResult == -1)
	{
		perror(strerror(errno));
		// set error
		std::cout << "binding failed\n";
		return false;
	}

	int requestQueueLength = 10;
	int listenResult = listen(mainServerSocketFd, requestQueueLength);

	if (listenResult == -1)
	{
		perror(strerror(errno));
		std::cout << "listen error\n";
		return false;
	}

	struct sockaddr_storage incomingConnectionAddress;
	socklen_t incomingConnectionAddressSize;

	int newSocketFd = accept(mainServerSocketFd, (struct sockaddr *)&incomingConnectionAddress, &incomingConnectionAddressSize);
	if (newSocketFd == -1)
	{
		perror(strerror(errno));
		std::cout << "accept error: new socket is not created!\n";
		return false;
	}

	std::string const messageToSend = "Hello from server\n";
	ssize_t numOfBytesSent = send(newSocketFd, messageToSend.c_str(), sizeof(messageToSend), 0);
	if (numOfBytesSent == -1)
	{
		perror(strerror(errno));
		std::cout << "send error\n";
		return false;
	}
	if (numOfBytesSent < static_cast<ssize_t>(sizeof(messageToSend)))
	{
		std::cerr << "the message is not sent completely";
	}

	char messageToRecieve[999999];
	ssize_t numOfBytesReceived = recv(newSocketFd, messageToRecieve, sizeof(messageToRecieve), 0);
	std::cout << "Received Message: " << messageToRecieve << std::endl;
	if (numOfBytesReceived == -1)
	{
		perror(strerror(errno));
		std::cout << "recieve error\n";
		return false;
	}

	if (numOfBytesReceived == 0)
	{
		perror(strerror(errno));
		std::cout << "connection is closed by the remote side (by the other side of the conenction)\n";
		return false;
	}

	// int connectResult = connect(mainServerSocketFd, res->ai_addr, res->ai_addrlen);
	// if (connectResult == -1)
	// {
	// 	std::cerr << strerror(errno) << std::endl;
	// 	std::cout << "not connected\n";
	// 	return false;
	// }

	std::cout << "OK\n";
	freeaddrinfo(res);
	close(mainServerSocketFd);
	close(newSocketFd);
	return true;
}