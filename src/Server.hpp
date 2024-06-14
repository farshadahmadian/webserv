/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:34:44 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/14 14:03:47 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <vector>
#include "Location.hpp"

class Server
{
	public:
		Server( void );
		~Server( void );

	private:
		bool 					is_default;
		std::string				server_name;
		std::string				listen;
		std::string				root;
		std::vector<Location>	location;
};

#endif /* __SERVER_HPP__ */
