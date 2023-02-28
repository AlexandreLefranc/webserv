#include "HTMLGenerator.hpp"

std::string	HTMLGenerator::error(int code, const std::string& desc)
{
	std::ostringstream	ss;

	ss << "<!doctype html>\n";
	ss << "<html>\n";
	ss << "  <body>\n";
	ss << "	<h1>\n";
	ss << "	  Error " << code << ": " << desc << "\n";
	ss << "	</h1>\n";
	ss << "  </body>\n";
	ss << "</html>\n";

	return ss.str();
}

std::string	HTMLGenerator::dirlist(const std::string& fullpath)
{
	std::vector<std::string> dirlist = get_dir_list(fullpath);

	std::ostringstream	ss;

	ss << "<!doctype html>\n";
	ss << "<html>\n";
	ss << "  <body>\n";
	ss << "	<h1>\n";
	ss << "	  Index of " << fullpath << "\n";
	ss << "	</h1>\n";
	ss << "	<hr>\n";

	std::vector<std::string>::const_iterator it;
	for (it = dirlist.begin(); it != dirlist.end(); ++it)
	{
		ss << "	";
		ss << "<a href=\"" << *it << "\">" << *it << "</a><br>\n";
	}

	ss << "	<hr>\n";
	ss << "  </body>\n";
	ss << "</html>\n";

	return ss.str();
}