#include "HTMLGenerator.hpp"

std::vector<char>	HTMLGenerator::error(int code, const std::string& desc)
{
	std::ostringstream	ss;
	std::string			buffer;
	std::vector<char>	content;

	ss << "<!doctype html>\n";
	ss << "<html>\n";
	ss << "  <body>\n";
	ss << "	<h1>\n";
	ss << "	  Error " << code << ": " << desc << "\n";
	ss << "	</h1>\n";
	ss << "  </body>\n";
	ss << "</html>\n";

	buffer = ss.str();
	content.reserve(buffer.size());
	content.assign(buffer.begin(), buffer.end());
	return (content);
}

std::vector<char>	HTMLGenerator::dirlist(const std::string& fullpath, const std::string& target)
{
	std::vector<std::string> dirlist = get_dir_list(fullpath);

	std::ostringstream	ss;
	std::string			buffer;
	std::vector<char>	content;

	ss << "<!doctype html>\n";
	ss << "<html>\n";
	ss << "  <body>\n";
	ss << "	<h1>\n";
	ss << "	  Index of " << target << "\n";
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

	buffer = ss.str();
	content.reserve(buffer.size());
	content.assign(buffer.begin(), buffer.end());
	return (content);
}