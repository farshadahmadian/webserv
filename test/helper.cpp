/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:58:37 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/15 13:08:54 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

std::string const getSocketType(struct addrinfo const *socketAddress) {
	int socketTypeMacro = (socketAddress->ai_socktype);
	// std::cout << "socketTypeMacro = " << socketTypeMacro << std::endl;
	
	switch (socketTypeMacro)
	{
	case SOCK_STREAM:
		return "TCP Socket";
	case SOCK_DGRAM:
		return "UDP Socket";
	default:
		return "Nither TCP or UDP Socket";
	}
}

std::string const getProtocolType(struct addrinfo const *socketAddress)
{
	int protocolTypeMacro = socketAddress->ai_protocol;

	switch (protocolTypeMacro)
	{
	case IPPROTO_TCP:
		return "TCP Protocol";
	case IPPROTO_UDP:
		return "UDP Protocol";
	default:
		return "Nither TCP or UDP Protocol";
	}
}

std::string const getFamilyType(struct addrinfo const *socketAddress)
{
	int familyTypeMacro = socketAddress->ai_family;

	switch (familyTypeMacro)
	{
	case AF_INET:
		return "IPv4";
	case AF_INET6:
		return "IPv6";
	default:
		return "Nither IPv4 or IPv6 Family";
	}
}