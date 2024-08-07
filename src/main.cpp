/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/06 12:51:59 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./parsing/Parser.hpp"
#include "./server/Poll.hpp"

int main(int argc, char **argv)
{
	try
	{
		Parser configuration(argc, argv);
		configuration.displayConf();
		std::cout << "---------------------------------------------------------" << std::endl;
		return (0);
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
	// close listening socket fds
	return (0);
}
