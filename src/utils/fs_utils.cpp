#include <dirent.h>

#include <algorithm>
#include <sstream>

#include "webserv.hpp"

static std::string	append_slash(std::string elem)
{
	return elem + "/";
}

std::vector<std::string> get_dir_list(std::string path)
{
	std::vector<std::string>	directories;
	std::vector<std::string>	regular_files;

	DIR* 			dir;
	struct dirent*	ent;

	dir = opendir(path.c_str());
	if (dir == NULL)
	{
		throw std::runtime_error("opendir() failed");
	}

	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_type == DT_DIR)
		{
			directories.push_back(ent->d_name);
		}
		else if (ent->d_type == DT_REG)
		{
			regular_files.push_back(ent->d_name);
		}
	}
	
	closedir(dir);

	std::transform(directories.begin(), directories.end(), directories.begin(), append_slash);

	std::sort(directories.begin(), directories.end());
	std::sort(regular_files.begin(), regular_files.end());

	directories.insert(directories.end(), regular_files.begin(), regular_files.end());
	return directories;
}


std::string		gen_dir_list_html(std::vector<std::string> dirlist, const std::string& target)
{
	std::ostringstream	ss;

	ss << "<!doctype html>\n";
	ss << "<html>\n";
	ss << "  <body>\n";
	ss << "    <h1>\n";
	ss << "      Index of " << target << "\n";
	ss << "    </h1>\n";
	ss << "    <hr>\n";

	std::vector<std::string>::const_iterator it;
	for (it = dirlist.begin(); it != dirlist.end(); ++it)
	{
		ss << "    ";
		ss << "<a href=\"" << *it << "\">" << *it << "</a><br>\n";
	}

	ss << "    <hr>\n";
	ss << "  </body>\n";
	ss << "</html>\n";

	return ss.str();
}