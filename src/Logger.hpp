/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:32:36 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/08 15:44:27 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGGER_HPP__
# define __LOGGER_HPP__

#include <string>

enum log {
	MINIMAL,
	STANDARD,
	DETAILED
};

class Logger
{
	public:
		Logger( void );
		~Logger( void );

		// static Logger& getLogger( void );
		
		enum log _level;


	
	private:
		Logger( const Logger& ); // ----------------------- UNUSED CONSTRUCTORS
		void operator=( const Logger& );
};

#endif /* __LOGGER_HPP__ */
