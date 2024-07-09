/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:32:45 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 15:43:46 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

// ---------------------------------------------------------- USED CONSTRUCTORS
Logger::Logger( void ) {
	if (VERBOSE == 0) _level = MINIMAL;
	if (VERBOSE == 1) _level = STANDARD;
	if (VERBOSE == 2) _level = DETAILED;
};
Logger::~Logger( void ) {};

// static Logger& getLogger( void ) {
// 	static Logger obj;
// 	return (obj);
// }

// -------------------------------------------------------- UNUSED CONSTRUCTORS
Logger::Logger( const Logger& ) {/* Not needed */};
void Logger::operator=( const Logger& ) {/* Not needed */};
