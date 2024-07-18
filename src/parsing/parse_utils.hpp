/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:12:05 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/18 14:06:09 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <string>
#include <iostream>

std::string& ltrim( std::string& s, const char* to_trim );
std::string& rtrim( std::string& s, const char* to_trim );
std::string& trim( std::string& s, const char* to_trim );

bool isValidServerName( std::string value );
bool isValidNumber( std::string key, std::string value );
bool isValidMethod( std::string s );

#endif /* __UTILS_HPP__ */
