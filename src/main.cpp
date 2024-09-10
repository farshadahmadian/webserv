/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/09/10 09:33:14 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./parsing/Parser.hpp"
#include "./server/Poll.hpp"

extern bool stopServer;

int main(int argc, char **argv)
{
	Server::logMessage("***** Run The Server *****");
	try
	{
		Parser configuration(argc, argv);
		// configuration.displayConf();
		// std::cout << "---------------------------------------------------------" << std::endl;
		Poll pollOfServers(configuration);
		pollOfServers.init();
		pollOfServers.start();
	}
	catch (Exception const &exception)
	{
		std::cerr << RED << exception.what() << RESET << std::endl;
		return exception.getError();
	}
	catch (std::exception &e)
	{
		std::cerr << R("Error: ") << e.what() << std::endl;
		return (1);
	}
	if (stopServer == true)
		return (1);
	return (0);
}
