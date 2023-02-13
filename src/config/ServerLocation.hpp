#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# include <fstream>

# include "parse_utils.hpp"

class ServerLocation
{
private:
	//	Attributes
	ifstream&					file;
	bool						exact_match;
	std::string					location_match;
	std::string					root;
	std::string					error_page;
	std::vector<std::string>	try_files;

public:
	//	Constructor
	ServerLocation(std::ifstream& file)
	ServerLocation(const ServerLocation& other);
	//	Destructor
	~ServerLocation();
	//	Assignment operator
	ServerLocation&	operator=(const ServerLocation& other);
};

#endif