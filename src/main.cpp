/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/13 14:29:37 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sockets/listening-socket/ListeningSocket.hpp"
#include "./sockets/connected-socket/ConnectedSocket.hpp"
#include "./parsing/Parser.hpp"
#include "./server/Poll.hpp"

int main(int argc, char **argv)
{
	Server::logMessage("***** Run The Server *****");
	try
	{
		Parser configuration(argc, argv);
		// configuration.displayConf();
		// std::cout << "---------------------------------------------------------" << std::endl;
		// return (0);
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

/*	LOCATION
	:8080/farshad	- ask file
	:8080/farshad/	- ask directory

	location
		uri /farshad; = match a file request
	location
		uri /farshad/; = match a folder request
	location
		uri farshad; = match what?
*/

/*	ROOT
root /var/www/
server
	root	./hello | ./hello/ | ../hello | every number of ../.././	=> translated to [folder-of-webserv]/hello/
	root	/hello 														=> translated to /hello (absolute path)
	root	hello														=> translated to /var/www/hello/ (appended to previous root)
*/

/*	ROOT + LOCATION
root /var/www/;
server
	root html;				=> translated to ./www/html/ (appended to previous root)
	location
		uri /farshad;
		root farshad.com	=> translated to ./www/html/farshad.com/farshad(file)
	location
		uri /farshad/;
		root farshad.com	=> translated to ./www/html/farshad.com/farshad/
	location
		uri farshad;
		root farshad.com	=> translated to ./www/html/farshad.com/
*/

/*	NGINX
	- root: the uri is appended to it
	- alias is different because the uri is not appended
*/
