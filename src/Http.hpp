/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/14 15:30:27 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTP_HPP__
# define __HTTP_HPP__

#include "Server.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <map>

#include <sys/stat.h>	// stat()
#include <cctype>		// isspace
#include "colors.h"

#define	SPACES			" \t\v\f\r"
#define	COMMENT(c)		((c) == '#')
#define	ENDVALUE(c)		((c) == ';')
#define	OPENBLOCK(c)	((c) == '{')
#define	CLOSEBLOCK(c)	((c) == '}')

enum file_err {
	E_TOOARGS,
	E_INVFILE,
	E_ISDIR,
	E_FAIL
};

enum parser_err {
	E_INVBLOCK,
	E_CONTEXTDECL,
	E_SYNTAX,
	E_INVPROP,
	E_ENDPROP
};

class Http
{
	public: // ---------------------------------------------- CONSTRUCTORS
		Http( int argc, char** argv );
		~Http( void );
	private:
		Http( void );
		Http( const Http& );
		void operator=( const Http& );
		
	private: // ---------------------------------------------- MEMBERS
		std::string keepalive_timeout;
		std::string client_body_buffer_size;
		std::map<std::string, std::string> directive;
		
		std::vector<Server> server;

		bool isDirectory( char* path ); // --------------------- PARSING
		void parse( std::ifstream& confFile );
		void parseContext( std::string& line, std::string& currBlock, std::string nextBlock );
		bool parseDirectives( std::string& line, std::string& currBlock );
		void addDirective( std::string& line, std::string& currBlock, const std::string dir );

		static const std::string httpDirectives[2];
		static const std::string serverDirectives[3];
		static const std::string locationDirectives[5];

		class FileExcept; // ------------------------------------- EXCEPTIONS
		class ParserExcept;
};

class Http::FileExcept: public std::exception
{
	private:
		int	_n;
	public:
		FileExcept( file_err n );
		const char* what() const throw();
};

class Http::ParserExcept: public std::exception
{
	private:
		int	_n;
	public:
		ParserExcept( parser_err n );
		const char* what() const throw();
};


#endif /* __HTTP_HPP__ */
