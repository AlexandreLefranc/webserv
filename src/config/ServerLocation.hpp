#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# include <fstream>
# include <pair>

# include "parse_utils.hpp"

class ServerLocation
{
private:
	//	Attributes
	sdt::ifstream&						file;
	bool								exact_match;
	std::string							location_match;
	std::string							root;
	std::vector<std::string>			index;
	std::vector<std::string>			try_files;
	std::pair<std::string, std::string>	error_page;

public:
	//	Constructor
	ServerLocation(std::ifstream& file, std::string& location_match, bool exact_match = false);
	ServerLocation(const ServerLocation& other);
	//	Destructor
	~ServerLocation();
	//	Assignment operator
	ServerLocation&	operator=(const ServerLocation& other);

private:
	void	_parse();
};

#endif