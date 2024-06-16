/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/16 15:00:29 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "colors.h"
#include "Http.hpp"
#include "test.hpp"
#include "StreamSocket.hpp"

int main ( int argc, char** argv ) {
	// std::cout << B("*************** WEBSERV! ***************") << std::endl;
	// try {
	// 	Http webserver(argc, argv);
	// } catch (std::exception& e) {
	// 	std::cerr << "Error: " << e.what() << std::endl;
	// 	return (1);
	// }

	// if (!createServer())
	// 	return 1;
	(void)argc;
	(void)argv;

	try {
		StreamSocket mainServerSideSocket;
		(void)mainServerSideSocket;

	} catch(StreamSocket::SocketException const &socketException) {
		std::cerr << RED << socketException.what() << RESET << std::endl;
		return socketException.getError();
	}
	return (0);
}
