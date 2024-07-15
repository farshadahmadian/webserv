/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:53:33 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 11:49:55 by ncasteln         ###   ########.fr       */
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

// ---------------------------------------------------------------------- UTILS
std::string& ltrim( std::string& s, const char* to_trim ) {
	size_t pos = s.find_first_not_of(to_trim);
	s.erase(0, pos);
	return (s);
}

std::string& rtrim( std::string& s, const char* to_trim ) {
	size_t pos = s.find_last_not_of(to_trim);
	s.erase(pos + 1);
	return (s);
}

std::string& trim( std::string& s, const char* to_trim ) {
	return (ltrim(rtrim(s, to_trim), to_trim));
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
		if (!isValidDirective(directive))
			throw ParseExcept(E_INVDIR, _line_counter);
		if (openContext(directive)) continue ;
		closeContext();																// if the current level of indentation is lower than the one before, one or two context are closed
		if (!isCorrectContextOpen())												// check if the context for {{{THAT}}} directive is open
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		value = extractValue(line);
		updateConfiguration(directive, value);
	}
	if (_http.getServer().size() == 0) throw ParseExcept(E_NOSERVER, _line_counter);
	_http.checkConfiguration();
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
	if (directive.compare("server") == 0 || directive.compare("location") == 0) {
		ltrim(line, SPACES);
		if (!line.empty() && !COMMENT(line[0]) && !ENDVALUE(line[0]))
			throw ParseExcept(E_CONTNAME, _line_counter);
	}
	return (directive);										// can be also a context names
}

bool Parser::isValidDirective( std::string directive ) {
	size_t size;
	const std::string* list;

	if (_currLvl == HTTP) {
		list = HttpConf::httpDirectives;
		size = N_HTTP_DIR;
	}
	if (_currLvl == SERVER) {
		list = ServerConf::serverDirectives;
		size = N_SERVER_DIR;
	}
	if (_currLvl == LOCATION) {
		list = LocationConf::locationDirectives;
		size = N_LOCATION_DIR;
	}
	for (size_t i = 0; i < size; i++) {
		if (list[i] == directive)
			return (true);
	}
	return (false);
}

/*	_activeContext is switched in case the portion of line read is "server" or
	"location". In those cases, an instance is also created.
*/
bool Parser::openContext( std::string directive ) {
	if (directive.compare("server") == 0) {
		_activeContext = SERVER;
		_http.addServer();
		return (true);
	}
	if (directive.compare("location") == 0) {
		if (_prevLvl == INIT)							// case context_2.conf
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		if (_prevLvl == HTTP && _currLvl != SERVER)		// case cut_server.conf
			throw ParseExcept(E_INVCONTEXT, _line_counter);
		_activeContext = LOCATION;
		_http.addLocation();
		return (true);
	}
	return (false);
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

void Parser::updateConfiguration( std::string directive, std::string value ) {
	// _http::setSetting(directive, value, _activeContext)

	if (_activeContext == HTTP)
		_http.setHttpSettings(directive, value);
	if (_activeContext == SERVER)
		_http.setServerSettings(directive, value);
	if (_activeContext == LOCATION)
		_http.setLocationSettings(directive, value);
}

// -------------------------------------------------------------------- GETTERS
HttpConf& Parser::getHttp( void ) { return (_http); };

// -------------------------------------------------------------------- DISPLAY
void Parser::displayParseState( std::string line ) {
	std::cout << "------------------------------------------------------------" << std::endl;
	std::cout << G("* [LINE]     ") << "[" << line << "]" << std::endl << std::endl;
	std::cout << B("* Prev indentation ") << "[ " << displayIndentantion(_prevLvl) << " ]" << std::endl;
	std::cout << B("* Curr indentation ") << "[ " << displayIndentantion(_currLvl) << " ]" << std::endl;
}

std::string Parser::displayIndentantion( indentation ind ) {
	if (ind == HTTP) return ("http");
	if (ind == SERVER) return ("server");
	if (ind == LOCATION) return ("location");
	return ("init");
}

void Parser::displayConf( void ) {
	std::cout << B("***************** { WEB SERVER CONFIGURATION } *****************") << std::endl;
	_http.displayHttpSettings();
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

Parser::ParseExcept::ParseExcept( parse_err n, int count ): _n(n), _line_counter(count) {};
const char* Parser::ParseExcept::what() const throw() {
	std::cerr << "line (" << _line_counter << ") ";
	if (_n == E_INVDIR) return("invalid conf: directive doesn't exist, or wrong indented");
	if (_n == E_CONTNAME) return("invalid conf: context name");
	if (_n == E_ONLYTABS) return("invalid conf: directive expected after tabs");
	if (_n == E_INVCONTEXT) return("invalid conf: directive declared in wrong context");
	if (_n == E_INVIND) return("invalid conf: wrong indentation");
	if (_n == E_ENDDECL) return("invalid conf: directive end not found");
	if (_n == E_EMPTYVAL) return("invalid conf: empty value");
	if (_n == E_NOSERVER) return("invalid conf: http has no server");
	if (_n == E_NOLOCATION) return("invalid conf: server has no location");
	return ("Unknkow exception");
}

// -------------------------------------------------------- UNUSED CONSTRUCTORS
Parser::Parser( void ) {/* Not needed */};
Parser::Parser( const Parser& ) {/* Not needed */};
void Parser::operator=( const Parser& ) {/* Not needed */};
