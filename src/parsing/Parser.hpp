/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 12:02:07 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 12:05:27 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
# define __PARSER_HPP__

#include "HttpConf.hpp"
#include <sys/stat.h>	// stat()
#include "../../include/errors.h"

#define DEFAULT_FILE_PATH	"./conf/default.conf"
#define	SPACES				" \t\v\f\r"
#define	COMMENT(c)			((c) == '#')
#define	ENDVALUE(c)			((c) == ';')

/* List of possible context directives */

enum indentation {
	INIT = -1,
	HTTP,
	SERVER,
	LOCATION,
	COMMENT
};

class Parser
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Parser( int argc, char** argv );
		~Parser( void );
		void displayConf( void );
		HttpConf& getHttp( void );

	private:
		HttpConf _http; // --------------------------------------------------- CONF

		indentation _prevLvl; // -------------------------------------- PARSING
		indentation _currLvl;
		indentation _activeContext;
		int _line_counter;

		void checkFile( int argc, char** argv, std::ifstream& );
		bool isDirectory( char* path );
		void parse( std::ifstream& confFile );
		void setCurrIndentation( std::string& line );
		std::string extractDirective( std::string& line );
		bool isValidDirective( std::string currDirective );
		bool openContext( std::string contextToOpen );
		void closeContext( void );
		bool isCorrectContextOpen( void );
		std::string extractValue( std::string& line );
		void updateConfiguration(std::string directive, std::string value);

		std::string displayIndentantion( indentation ); // ------------ DISPLAY
		void displayParseState( std::string line );

		class FileExcept; // --------------------------------------- EXCEPTIONS
		class ParseExcept;

		Parser( void ); // ---------------------------------- UNUSED CONSTRUCTORS
		Parser( const Parser& );
		void operator=( const Parser& );
};

class Parser::FileExcept: public std::exception
{
	private:
		int	_n;
	public:
		FileExcept( file_err n );
		const char* what() const throw();
};

class Parser::ParseExcept: public std::exception
{
	private:
		int	_n;
		int	_line_counter;
	public:
		ParseExcept( parse_err n, int line_counter );
		const char* what() const throw();
};

#endif /* __PARSER_HPP__ */
