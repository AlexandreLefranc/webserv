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
	static std::vector<char>	error(int code, const std::string& desc);
	static std::vector<char>	dirlist(const std::string& fullpath, const std::string& target);
};

#endif