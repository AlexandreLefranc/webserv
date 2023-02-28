#ifndef HTMLGENERATOR_HPP
# define HTMLGENERATOR_HPP

# include <sstream>
# include <string>
# include <vector>

# include "webserv.hpp"

class HTMLGenerator
{
private: // Non instanciable
	HTMLGenerator();

public:
	static std::string	error(int code, const std::string& desc);
	static std::string	dirlist(const std::string& fullpath);
};

#endif