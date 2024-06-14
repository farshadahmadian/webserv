/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:32:58 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/14 13:45:11 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

#include <string>

class Location
{
	public:
		Location( void );
		~Location( void );

	private:
		std::string	root;
		std::string	index;
		std::string	redirect_to;
		std::string	method;
		bool		autoindex;
		std::string	cgi;
};

#endif /* __LOCATION_HPP__ */
