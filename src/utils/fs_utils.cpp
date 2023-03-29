#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <sstream>
#include <vector>
#include <ios>
#include <fstream>
#include "webserv.hpp"
#include "Exceptions.hpp"

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

std::vector<char>	read_file(std::ifstream& ifs)
{
	size_t	file_size;

	ifs.seekg(0, std::ios::end);
	file_size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<char>	content(file_size);

	ifs.read(reinterpret_cast<char *>(&content[0]), file_size);
	return (content);
}

bool	file_exists(const std::string& filename)
{
	if (access(filename.c_str(), F_OK) == 0)
		return (true);
	else
		return (false);
}

bool	is_directory(const std::string& path)
{
	struct stat	s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return (true);
		else
			return (false);
	}
	std::cout << "stat failed." << std::endl;
	return (false);
}