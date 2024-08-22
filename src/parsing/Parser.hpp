/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 12:02:07 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/13 14:30:10 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
# define __PARSER_HPP__

#include "HttpConf.hpp"
# include "Server.hpp"

class Parser
{
	public: // ---------------------------------------------- USED CONSTRUCTORS
		Parser( int argc, char** argv );
		~Parser( void );
		Parser( const Parser& );

		void displayConf( void );
		const HttpConf& getHttp( void ) const;

	private:
		HttpConf _http; // ----------------------------------------------- CONF

		context _prevLvl; // ------------------------------------------ PARSING
		context _currLvl;
		context _activeContext;
		int _line_counter;

		void checkFile( int argc, char** argv, std::ifstream& );
		bool isDirectory( char* path );
		void parse( std::ifstream& confFile );
		void setCurrIndentation( std::string& line );
		std::string extractDirective( std::string& line );
		bool isValidContext( std::string directive );
		bool isValidSetting( std::string directive );
		bool isDuplicate( std::string directive );
		void openContext( std::string contextToOpen );
		void closeContext( void );
		bool isCorrectContextOpen( void );
		std::string extractValue( std::string& line );

		std::string displayIndentantion( context ); // ---------------- DISPLAY
		void displayParseState( std::string line );

		class FileExcept; // --------------------------------------- EXCEPTIONS
		class ParseExcept;
		class ConfExcept;

		Parser( void ); // -------------------------------- UNUSED CONSTRUCTORS
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

class Parser::ConfExcept: public std::exception
{
	private:
		int	_n;
		int	_line_counter;
	public:
		ConfExcept( conf_err n, int	line_counter );
		const char* what() const throw();
};

#endif /* __PARSER_HPP__ */
