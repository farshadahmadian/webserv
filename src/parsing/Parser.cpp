/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:53:33 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/13 14:29:18 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

// --------------------------------------------------------------- CONSTRUCTORS
Parser::~Parser( void ) {};
Parser::Parser( int argc, char** argv ):
	_prevLvl(INIT),
	_currLvl(INIT),
	_activeContext(HTTP),
	_line_counter(0) {

	std::ifstream confFile;
	checkFile(argc, argv, confFile);
	parse(confFile);
	confFile.close();
};
Parser::Parser( const Parser& obj ) {
	Server::logMessage("Parse The Input In The Parser Constructor!");

	this->_http = obj._http;
	this->_prevLvl = obj._prevLvl;
	this->_currLvl = obj._currLvl;
	this->_activeContext = obj._activeContext;
	this->_line_counter = obj._line_counter;
}

// -------------------------------------------------------------------- PARSING
void Parser::checkFile( int argc, char** argv, std::ifstream& confFile ) {
	if (argc > 2) throw FileExcept(E_TOOARGS);
	if (!argv[1] || std::string(argv[1]).empty())
		confFile.open(DEFAULT_FILE_PATH, std::ifstream::in);
	else {
		confFile.open(argv[1], std::ifstream::in);
		if (!confFile) throw FileExcept(E_INVFILE);
		if (isDirectory(argv[1])) throw FileExcept(E_ISDIR);
	}
	if (!confFile) throw FileExcept(E_INVFILE);
}

bool Parser::isDirectory( char* path ) {
	struct stat s;
	if (stat(path, &s) == 0) {
		if (S_ISDIR(s.st_mode))
			return (true);
	}
	else
		return (true);
	return (false);
}

void Parser::parse( std::ifstream& confFile ) {
	std::string line;
	std::string directive;
	std::string value;

	while(getline(confFile, line, '\n')) {
		_line_counter++;
		if (confFile.fail())
			throw FileExcept(E_FAIL);
		if (line.empty()) continue ;												// empty lines are jumped
		if (line.find_first_not_of(SPACES) == std::string::npos) continue ;			// line with only spaces and IND don't do nothing neither changing level
		setCurrIndentation(line);													// set curr level based on line TABS
		if (_currLvl == COMMENT) continue;
		directive = extractDirective(line);											// extract the directive
		if (isValidContext(directive)) {
			openContext(directive);
			continue;
		}
		if (!isValidSetting(directive))
			throw ParseExcept(E_INVDIR, _line_counter);
		closeContext();																// if the current level of indentation is lower than the one before, one or two context are closed
		if (!isCorrectContextOpen())												// check if the context for {{{THAT}}} directive is open
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		value = extractValue(line);
		_http.setSetting(directive, value, _activeContext);
	}
	/*
		0) handle roots
		1) Autocomplete with left settings
		2) Check the configuration if something is wrong
		3) clean the slashes ???
	*/
	if (_http.getServer().size() == 0) throw ParseExcept(E_NOSERVER, _line_counter);
	_http.setDefaults();
	// conf_err n = _http.checkSettings();
	// if (n) throw ConfExcept(n, _line_counter);
}

/*	Get the number of tabs preceding the line and erase them. To detect some
	wrong nesting, we need to store the value of the previous context.
*/
void Parser::setCurrIndentation( std::string& line ) {
	if (_currLvl != COMMENT && _currLvl != INIT)
		_prevLvl = _currLvl;
	int i = 0;
	while (line[static_cast<size_t>(i)] == '\t')
		i++;
	line.erase(0, static_cast<size_t>(i));
	if (i == 0) _currLvl = HTTP;
	if (i == 1) _currLvl = SERVER;
	if (i == 2) _currLvl = LOCATION;
	if (COMMENT(line[0]) || ENDVALUE(line[0])) {
		_currLvl = COMMENT;
		return ;
	}
	if (i > 2)
		throw ParseExcept(E_INVIND, _line_counter);
}

/*	Extract the directive name and erases it. It checks also if after "server"
	and "location" is there something dirty.
*/
std::string Parser::extractDirective( std::string& line ) {
	if (line.empty()) throw ParseExcept(E_ONLYTABS, _line_counter);
	size_t firstSpace = line.find_first_of(SPACES);			// go to first space
	std::string directive = line.substr(0, firstSpace);		// cut the string
	line.erase(0, directive.length());
	if (directive == "server" || directive == "location") {
		ltrim(line, SPACES);
		if (!line.empty() && !COMMENT(line[0]) && !ENDVALUE(line[0]))
			throw ParseExcept(E_CONTNAME, _line_counter);
	}
	return (directive);										// can be also a context names
}

bool Parser::isValidContext( std::string directive ) {
	if (_currLvl == HTTP && directive == "server")
		return (true);
	if (_currLvl == SERVER && directive == "location")
		return (true);
	return (false);
}

bool Parser::isValidSetting( std::string directive ) {
	const std::string* sharedList = AConf::sharedSettings;
	for (size_t i = 0; i < N_SHARED_DIR; i++) {
		if (sharedList[i] == directive)
			return (true);
	}
	if (_currLvl == HTTP)
		return (false);

	size_t size;
	const std::string* specificList;
	if (_currLvl == SERVER) {
		specificList = ServerConf::serverSettings;
		size = N_SERVER_DIR;
	}
	if (_currLvl == LOCATION) {
		specificList = LocationConf::locationSettings;
		size = N_LOCATION_DIR;
	}
	for (size_t i = 0; i < size; i++) {
		if (specificList[i] == directive)
			return (true);
	}
	return (false);
}

/*	_activeContext is switched in case the portion of line read is "server" or
	"location". In those cases, an instance is also created.
*/
void Parser::openContext( std::string directive ) {
	if (directive == "server")
		_activeContext = SERVER;
	else if (directive == "location") {
		if (_prevLvl == INIT)							// case context_2.conf
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		if (_prevLvl == HTTP && _currLvl != SERVER)		// case cut_server.conf
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		_activeContext = LOCATION;
	}
	_http.addNestedBlock(_activeContext);
}

/*	If the currLvl of indentation is lower than the one of the active context,
	the active context is closed and becomes the one of the currLvl.
*/
void Parser::closeContext( void ) {
	if (_currLvl < _activeContext)
		_activeContext = _currLvl;
}

/*	The active context and the current level of indentation has to be always
	equal, otherwise it means, that a directive was written in the wrong one.
*/
bool Parser::isCorrectContextOpen( void ) {
	if (_activeContext != _currLvl)
		return (false);
	return (true);
}

std::string Parser::extractValue( std::string& line ) {
	size_t end_of_decl;
	std::string value;

	ltrim(line, SPACES);
	end_of_decl = line.find(';');
	if (end_of_decl == std::string::npos)
		throw ParseExcept(E_ENDDECL, _line_counter);
	value = line.substr(0, end_of_decl);
	if (value.empty())
		throw ParseExcept(E_EMPTYVAL, _line_counter);
	rtrim(value, SPACES);
	return (value);
}

const HttpConf& Parser::getHttp( void ) const { return(_http); };

// -------------------------------------------------------------------- DISPLAY
void Parser::displayParseState( std::string line ) {
	std::cout << "------------------------------------------------------------" << std::endl;
	std::cout << G("* [LINE]     ") << "[" << line << "]" << std::endl << std::endl;
	std::cout << B("* Prev indentation ") << "[ " << displayIndentantion(_prevLvl) << " ]" << std::endl;
	std::cout << B("* Curr indentation ") << "[ " << displayIndentantion(_currLvl) << " ]" << std::endl;
}

std::string Parser::displayIndentantion( context ind ) {
	if (ind == HTTP) return ("http");
	if (ind == SERVER) return ("server");
	if (ind == LOCATION) return ("location");
	return ("init");
}

void Parser::displayConf( void ) {
	std::cout << B("***************** { WEB SERVER CONFIGURATION } *****************") << std::endl;
	_http.displaySettings();
}

// ----------------------------------------------------------------- EXCEPTIONS
Parser::FileExcept::FileExcept( file_err n ): _n(n) {};
const char* Parser::FileExcept::what() const throw() {
	if (_n == E_TOOARGS) return ("too many args");
	if (_n == E_INVFILE) return ("invalid file");
	if (_n == E_ISDIR) return ("arg provided is a directory");
	if (_n == E_FAIL) return ("fail reading the file");
	return ("Unknow File exception");
}

Parser::ParseExcept::ParseExcept( parse_err n, int line_counter ): _n(n), _line_counter(line_counter) {};
const char* Parser::ParseExcept::what() const throw() {
	std::cerr << "line (" << _line_counter << ") invalid conf: ";
	if (_n == E_INVDIR) return("directive doesn't exist, or wrong indented");
	if (_n == E_CONTNAME) return("invalid context name");
	if (_n == E_ONLYTABS) return("directive expected after tabs");
	if (_n == E_INVCONTEXT) return("directive declared in wrong context");
	if (_n == E_INVIND) return("wrong indentation");
	if (_n == E_ENDDECL) return("directive end not found");
	if (_n == E_EMPTYVAL) return("empty value");
	if (_n == E_NOSERVER) return("http has no server");
	if (_n == E_NOLOCATION) return("server has no location");
	if (_n == E_DUPLICATE) return("duplicate directive detected");
	return ("Unknkow Parse exception");
}

Parser::ConfExcept::ConfExcept( conf_err n, int line_counter ): _n(n), _line_counter(line_counter) {};
const char* Parser::ConfExcept::what() const throw() {
	std::cerr << "line (" << _line_counter << ") invalid conf: ";
	if (_n == E_AUTOINDEX) return("autoindex can be only `on` or `off`");
	if (_n == E_TIMEOUT) return("keepalive_timeout can only be a number between `???` and `???`");
	if (_n == E_CLIENTSIZE) return("client_body_buffer_size can only be a number between `???` and `???`");
	if (_n == E_HOST) return("wrong ip format");
	if (_n == E_PORT) return("port can only be a number between `???` and `???`");
	if (_n == E_METHOD) return("only `GET` `POST` `DELETE` methods are allowed");
	return ("Unknkow Conf exception");
}
