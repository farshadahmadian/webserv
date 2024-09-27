/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   createClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:01:36 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/13 10:41:30 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <unistd.h>

/*	This snippet creates .hpp and .cpp files for class implementation, in the
	current directory in which is ran. If a file exists, it's not overwritten.
	The methods implemented are constructor, destructor, copy constructor and
	oeprator=.
*/
void createHeaderFile( std::string name ) {
	std::ofstream outfile;
	outfile.open(name + ".hpp");
	std::cout << "* Creating [" << name+".hpp" << "] file..." << std::endl;

	std::string guard(name);
	std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);

	outfile
	<< "#ifndef " << "__" << guard << "_HPP__\n"
	<< "# define " << "__" << guard << "_HPP__\n\n"
	<< "class " << name << std::endl
	<< "{\n"
	<< "	public:\n"
	<< "		" << name << "( void ); // -------------------------------------------- CANONICAL\n"
	<< "		" << "~" << name << "( void );\n"
	<< "		" << name << "( const " << name << "& );\n"
	<< "		" << name << "& operator=" << "( const " << name << "& );\n"
	<< "	private:\n"
	<< "		// Needs definitions\n"
	<< "};\n\n"
	<< "#endif " << "/* __" << guard << "_HPP__ */\n"
	<< std::endl;

	outfile.close();
}

void createImplementationFile( std::string name ) {
	std::ofstream outfile;
	outfile.open(name + ".cpp");
	std::cout << "* Creating [" << name+".cpp" << "] file..." << std::endl;

	outfile
	<<	"#include \"" << name << ".hpp\"\n\n"
	<< "// ------------------------------------------------------------------ CANONICAL\n"
	<<	name << "::" << name << "( void ) {};\n"
	<<	name << "::~" << name << "( void ) {};\n"
	<<	name << "::" << name << "( const " << name << "& obj ) " << "{\n"
	<<	"	// Need implementation\n"
	<<	"};\n"
	<<	name << "& " << name << "::operator=" << "( const " << name << "& rhs ) " << "{\n"
	<<	"	// Need implementation\n"
	<<	"};\n"
	<< std::endl;

	outfile.close();
}

bool fileExist( std::string fname ) {
	std::ofstream file;
	bool exists = access(fname.c_str(), F_OK) == 0;
	file.close();
	if (exists)
		return (true);
	return (false);
}

int main ( int argc, char** argv ) {
	if (argc < 2) {
		std::cerr << "Error: needs at least one arg" << std::endl;
		return (1);
	}

	for (int i = 1; i < argc; i++) {
		std::string name = argv[i];
		if (name.empty())
			continue ;
		if (fileExist(name+".hpp") || fileExist(name+".cpp")) {
			std::cerr << "Error: file already exists" << std::endl;
			return (1);
		}
		createHeaderFile(name);
		createImplementationFile(name);
	}
	return (0);
}
