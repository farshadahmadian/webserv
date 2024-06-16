/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:24:40 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/16 07:48:45 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "colors.h"
#include "Http.hpp"
#include "test.hpp"

int main ( int argc, char** argv ) {
	// std::cout << B("*************** WEBSERV! ***************") << std::endl;
	// try {
	// 	Http webserver(argc, argv);
	// } catch (std::exception& e) {
	// 	std::cerr << "Error: " << e.what() << std::endl;
	// 	return (1);
	// }

	if (!createServer())
		return 1;
	return (0);
}
