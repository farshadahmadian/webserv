#include "HttpServer.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"

void memoryTest () {
	HttpServer server1;
	HttpServer server2(20, "127.0.0.1", "8081");
	HttpServer server3(server2);
	server1 = server2;
	server2 = server3;

	Socket socket1;
	Socket socket2(200);
	Socket socket3(socket2);
	socket1 = socket2;
	socket2 = socket3;

	ListeningSocket listeningSocket1;
	ListeningSocket listeningSocket2(10, "127.0.0.1", "8080");
	ListeningSocket listeningSocket3(listeningSocket1);
	listeningSocket1 = listeningSocket2;
	listeningSocket2 = listeningSocket3;

	// addrinfo *addressInfo = listeningSocket1.allocateAddrInfo();
	listeningSocket1.createSocket();
	server1.bindSocket();
	server1.listenToRequests();
	server1.acceptFirstRequestInQueue();
	ConnectedSocket connectedSocket2;
	ConnectedSocket connectedSocket1;
	ConnectedSocket connectedSocket3(connectedSocket2);
	ConnectedSocket connectedSocket4(201, connectedSocket1.getIncomingRequestAddress(), connectedSocket1.getIncomingConnectionAddressSize());
	connectedSocket3 = connectedSocket1;

	// freeaddrinfo(addressInfo);
	return;
}