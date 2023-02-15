#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <fstream>

# include "parse_utils.hpp"

class ServerConfig
{
private:
	//	Attribute
	std::ifstream&				file;
	std::ofstream				log_stream;
	std::string					root;
	std::string					server_name;
	std::pair<int, short>		listen_port = make_pair(, 80);
	std::vector<std::string>	index;
	std::vector<ServerLocation>	locations;

public:
	// Member functions
	//		Constructor
	HTTPConfig(std::ifstream& config);
	HTTPConfig(const HTTPConfig& other);
	//		Destructor
	~HTTPConfig();
	//Assignment operator
	HTTPConfig&	operator=(const HTTPConfig& other);

	//	Location finder
	std::string	find_location(std::string path);

private:
	//	Parsing functions
	void	_parse();
	void	_parse_line(std::string& line);
	void
};
