/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:22:06 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/20 10:26:55 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONF_HPP__
# define __CONF_HPP__

#include "Http.hpp"
#include <sys/stat.h>	// stat()
#include "errors.h"

#define DEFAULT_FILE_PATH	"./conf/default.conf"
#define	SPACES				" \t\v\f\r"
#define	COMMENT(c)			((c) == '#')
#define	ENDVALUE(c)			((c) == ';')

/* List of possible context directives */
#define N_HTTP_DIR		3
#define N_SERVER_DIR	4
#define N_LOCATION_DIR	6

enum indentation {
	INIT = -1,
	HTTP,
	SERVER,
	LOCATION,
	COMMENT
};

class Conf
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Conf( int argc, char** argv );
		~Conf( void );
		void displayConf( void );

	private:
		Http _http; // --------------------------------------------------- CORE

		indentation _prevLvl; // -------------------------------------- PARSING
		indentation _currLvl;
		indentation _activeContext;
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
		void checkConfiguration( void );

		std::string displayIndentantion( indentation ); // ------------ DISPLAY
		void displayParseState( std::string line );

		static const std::string _httpDirectives[N_HTTP_DIR]; // ---- DIR LISTS
		static const std::string _serverDirectives[N_SERVER_DIR];
		static const std::string _locationDirectives[N_LOCATION_DIR];

		class FileExcept; // --------------------------------------- EXCEPTIONS
		class ParseExcept;

		Conf( void ); // ---------------------------------- UNUSED CONSTRUCTORS
		Conf( const Conf& );
		void operator=( const Conf& );
};

class Conf::FileExcept: public std::exception
{
	private:
		int	_n;
	public:
		FileExcept( file_err n );
		const char* what() const throw();
};

class Conf::ParseExcept: public std::exception
{
	private:
		int	_n;
	public:
		ParseExcept( parse_err n );
		const char* what() const throw();
};

#endif /* __CONF_HPP__ */
