/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/19 16:43:30 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTP_HPP__
# define __HTTP_HPP__

#ifndef	SILENT
# define SILENT	0
#endif

#include "Server.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <map>

#include <sys/stat.h>	// stat()
#include <cctype>		// isspace
#include "colors.h"

#define DEFAULT_FILE_PATH	"./conf/default.conf"

#define	SPACES			" \t\v\f\r"
#define	NEWLINE(c)		((c) == '\n')
#define	COMMENT(c)		((c) == '#')
#define	ENDVALUE(c)		((c) == ';')

#define N_HTTP_DIR		3
#define N_SERVER_DIR	4
#define N_LOCATION_DIR	6

enum file_err {
	E_TOOARGS = 110,
	E_INVFILE,
	E_ISDIR,
	E_FAIL
};

enum parser_err {
	E_ONLYTABS = 114,
	E_INVDIR,
	E_INVCONTEXT,
	E_INVIND,
	E_ENDDECL,
};

enum indentation {
	INIT = -1,
	HTTP,
	SERVER,
	LOCATION
};

class Http
{
	public: // -------------------------------------------- PUBLIC CONSTRUCTORS
		Http( int argc, char** argv );
		~Http( void );
	private:  // ----------------------------------------- PRIVATE CONSTRUCTORS
		Http( void );
		Http( const Http& );
		void operator=( const Http& );
		
	public: // ------------------------------------------------- PUBLIC MEMBERS
	
	private: // ----------------------------------------------- PRIVATE MEMBERS
		size_t _n_server;
		std::vector<Server> _server;
		std::map<std::string, std::string> _settings;

		bool isDirectory( char* path ); // ---------------------------- PARSING
		void parse( std::ifstream& confFile );
		
		indentation _prevLvl;
		indentation _currLvl;
		indentation _activeContext;
		void setCurrIndentation( std::string& line );
		std::string displayIndentantion( indentation );
		
		
		std::string getDirective( std::string& line );
		bool isValidDirective( std::string currDirective );
		
		bool openContext( std::string contextToOpen );
		void closeContext( void );
		bool isCorrectContextOpen( void );
		std::string getValue( std::string directive, std::string& line );

		void storeDirective(std::string directive, std::string value);

		void displayConfiguration( void );

		static const std::string _httpDirectives[N_HTTP_DIR]; // ---------- DIRECTIVE LISTS
		static const std::string _serverDirectives[N_SERVER_DIR];
		static const std::string _locationDirectives[N_LOCATION_DIR];

		class FileExcept; // --------------------------------------- EXCEPTIONS
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
