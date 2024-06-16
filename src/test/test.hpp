/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 11:44:46 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/16 10:44:21 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <netdb.h> // for structs like "struct addrinfo", and macros
#include <iostream>
// #include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa() ==> must be removed and ntohs()
#include <string>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

bool createServer(void);
std::string const getSocketType(struct addrinfo const *socketAddress);
std::string const getProtocolType(struct addrinfo const *socketAddress);
std::string const getFamilyType(struct addrinfo const *socketAddress);