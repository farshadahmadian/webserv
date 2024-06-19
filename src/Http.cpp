/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:32:42 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/19 17:02:19 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

// ------------------------------------------------------------------ CANONICAL
Http::Http( void ) {};
Http::~Http( void ) {};
Http::Http( const Http& obj ) { (void)obj;/* Not implemented */};
void Http::operator=( const Http& rhs ) { (void)rhs;/* Not implemented */};

// ---------------------------------------------------------- PARAM CONSTRUCTOR
Http::Http( int argc, char** argv ): 
	_n_server(0),
	_prevLvl(HTTP),
	_currLvl(HTTP),
	_activeContext(HTTP) {

	if (argc > 2) throw FileExcept(E_TOOARGS);

	std::ifstream confFile;
	if (!argv[1] || std::string(argv[1]).empty())
		confFile.open(DEFAULT_FILE_PATH, std::ifstream::in);
	else {
		confFile.open(argv[1], std::ifstream::in);
		if (!confFile) throw FileExcept(E_INVFILE);
		if (isDirectory(argv[1])) throw FileExcept(E_ISDIR);
	}
	if (!confFile) throw FileExcept(E_INVFILE);
	parse(confFile);
	confFile.close();
};

bool Http::isDirectory( char* path ) {
	struct stat s;
	if (stat(path, &s) == 0) {
		if (S_ISDIR(s.st_mode))
			return (true);
	}
	else
		return (true);
	return (false);
}

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
void Http::parse( std::ifstream& confFile ) {	
	std::string line;
	std::string directive;
	std::string value;

	while(getline(confFile, line, '\n')) {
		if (confFile.fail()) throw FileExcept(E_FAIL);
		
		if (line.empty()) continue ;							// empty lines are jumped
		if (COMMENT(line[0]) || ENDVALUE(line[0])) continue ;	// comment lines are jumped

		// store the prev level
		setCurrIndentation(line); // set curr level based on line TABS
		std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << G("* [LINE]     ") << "[" << line << "]" << std::endl;
		std::cout << B("* [PREV IND] ") << "[ " << displayIndentantion(_prevLvl) << " ]" << std::endl;
		std::cout << B("* [CURR IND] ") << "[ " << displayIndentantion(_currLvl) << " ]" << std::endl;

		directive = getDirective(line);
		if (!isValidDirective(directive))						// the directive exists
			throw ParserExcept(E_INVDIR);
		if (openContext(directive))
			continue ;
		closeContext();												// if the current level of indentation is lower than the one before, one or two context are closed 
		if (!isCorrectContextOpen())			// check if the context for {{{THAT}}} directive is open
			throw ParserExcept(E_INVCONTEXT);
		value = getValue(directive, line);
		storeDirective(directive, value);
	}
	displayConfiguration();
}

/*	Get the number of tabs preceding the line and erase them. To detect some 
	wrong nesting, we need to store the value of the previous context.
*/
void Http::setCurrIndentation( std::string& line ) {
	_prevLvl = _currLvl;
	int i = 0;
	while (line[static_cast<size_t>(i)] == '\t')
		i++;
	line.erase(0, static_cast<size_t>(i));
	if (i == 0) _currLvl = HTTP;
	if (i == 1) _currLvl = SERVER;
	if (i == 2) _currLvl = LOCATION;
	if (i > 2) throw ParserExcept(E_INVIND);
}

std::string Http::displayIndentantion( indentation ind ) {
	if (ind == HTTP) return ("http");
	if (ind == SERVER) return ("server");
	if (ind == LOCATION) return ("location");
	return ("init");
}

std::string Http::getDirective( std::string& line ) {
	if (line.empty())
		throw ParserExcept(E_ONLYTABS);
	size_t firstSpace = line.find_first_of(SPACES);				// go to first space
	// if not exist throw error ---- NO because already checked in isValidDirective
	std::string directive = line.substr(0, firstSpace);		// cut the string
	return (directive); // can be also a context names
}

bool Http::isValidDirective( std::string directive ) {	
	size_t size;
	const std::string* list;

	if (_currLvl == HTTP) {
		list = _httpDirectives;
		size = N_HTTP_DIR;
	}
	if (_currLvl == SERVER) {
		list = _serverDirectives;
		size = N_SERVER_DIR;
	}
	if (_currLvl == LOCATION) {
		list = _locationDirectives;
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
bool Http::openContext( std::string directive ) {
	if (directive == "server") {
		_activeContext = SERVER;
		std::cout << G("* [ACTIVATED CONTEXT]: ") << displayIndentantion(_activeContext);
		
		Server s(_n_server);
		_server.push_back(s); // copy constructor
		_n_server++;
		
		std::cout << G(" - index: ") << _server.back().getIndex() << std::endl;
		return (true);
	} 
	if (directive == "location") {
		if (_prevLvl == HTTP)
			throw ParserExcept(E_INVCONTEXT);
		_activeContext = LOCATION;
		std::cout << G("* [ACTIVATED CONTEXT]: ") << displayIndentantion(_activeContext) << std::endl;

		std::cout << Y("* [ADDING LOCATION to server number: ") << _server.back().getIndex() << std::endl;
		_server.back().addLocation(); // MISTAKE ???? seems ok
		
		std::cout << G("* [index of new location]: ") << _server.back().getLocation().back().getIndex() << std::endl;
		return (true);
	}
	return (false);
}

/*	If the currLvl of indentation is lower than the one of the active context,
	the active context is closed and becomes the one of the currLvl.
*/
void Http::closeContext( void ) {
	if (_currLvl < _activeContext) {
		std::cout << R("* [CLOSED CONTEXT]: ") << displayIndentantion(_activeContext) << std::endl;
		_activeContext = _currLvl;
		std::cout << G("* [ACTIVE CONTEXT]: ") << displayIndentantion(_activeContext) << std::endl;
	}
}

/*	The active context and the current level of indentation has to be always
	equal, otherwise it means, that a directive was written in the wrong one.
*/
bool Http::isCorrectContextOpen( void ) {
	if (_activeContext != _currLvl)
		return (false);
	return (true);
}

std::string Http::getValue( std::string directive, std::string& line ) {
	size_t end_of_decl;
	std::string value;
	
	line.erase(0, directive.length());
	ltrim(line, SPACES);
	end_of_decl = line.find(';');
	if (end_of_decl == std::string::npos)
		throw ParserExcept(E_ENDDECL);
	value = line.substr(0, end_of_decl);
	rtrim(value, SPACES);
	return (value);
}

void Http::storeDirective(std::string directive, std::string value) {
	std::cout << R("* STORING DIRECTIVE: ");
	if (_activeContext == HTTP) {
		std::cout << "-----[ HTTP ] " << directive << " - " << value << std::endl;
		_settings[directive] = value;
	}
	if (_activeContext == SERVER) {
		std::cout << "-----[ SERVER " << _server.back().getIndex() << " ] " << directive << " - " << value << std::endl;
		_server.back().setSettings(directive, value);
	}
	if (_activeContext == LOCATION) {
		std::cout << "-----[ LOCATION ] " << directive << " - " << value << std::endl;
		// maybe add a guard in case size == 0 ?
		_server.back().getLocation().back().setSettings(directive, value);
	}
	
}

// -------------------------------------------------------------------- DISPLAY
void Http::displayConfiguration( void ) {
	std::cout << B("******** { WEB SERVER CONFIGURATION } ********") << std::endl;
	std::cout << G("[HTTP]") << std::endl;
	std::map<std::string, std::string>::iterator httpIt = _settings.begin();
	while (httpIt != _settings.end()) {
		std::cout << "  * " << (*httpIt).first << ": " << (*httpIt).second << std::endl;
		httpIt++;
	}
	std::vector<Server>::iterator serverIt = _server.begin();
	while (serverIt != _server.end()) {
		(*serverIt).displayServerSettings();
		serverIt++;
	}
}


// ------------------------------------------------------------ FILE EXCEPTIONS
Http::FileExcept::FileExcept( file_err n ): _n(n) {};
const char* Http::FileExcept::what() const throw() {
	if (_n == E_TOOARGS) return ("too many args");
	if (_n == E_INVFILE) return ("invalid file");
	if (_n == E_ISDIR) return ("arg provided is a directory");
	if (_n == E_FAIL) return ("fail reading the file");
	return ("Unknow File exception");
}

// ----------------------------------------------------------- PARSE EXCEPTIONS
Http::ParserExcept::ParserExcept( parser_err n ): _n(n) {};
const char* Http::ParserExcept::what() const throw() {
	if (_n == E_INVDIR) return("invalid conf: directive does't exist");
	if (_n == E_ONLYTABS) return("invalid conf: directive expected after tabs");
	if (_n == E_INVCONTEXT) return("invalid conf: directive declared in wrong context");
	if (_n == E_INVIND) return("invalid conf: invalid indentation");
	if (_n == E_ENDDECL) return("invalid conf: directive end not found");
	return ("Unknkow exception");
}

// -------------------------------------------------------------------- STATICS
const std::string Http::_httpDirectives[N_HTTP_DIR] = {
	"keepalive_timeout",
	"client_body_buffer_size",
	"server" };
const std::string Http::_serverDirectives[N_SERVER_DIR] = {
	"server_name",
	"listen",
	"root",
	"location" };
const std::string Http::_locationDirectives[N_LOCATION_DIR] = {
	"uri", 
	"root", 
	"index", 
	"method", 
	"autoindex", 
	"cgi" };
