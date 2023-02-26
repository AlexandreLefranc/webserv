#ifndef HTMLGENERATOR_HPP
# define HTMLGENERATOR_HPP

# include <sstream>
# include <string>
# include <vector>

# include "webserv.hpp"

struct HTMLGenerator
{
	std::string	error(int code, const std::string& desc) const;

	std::string	dirlist(const std::string& fullpath) const;
};

#endif