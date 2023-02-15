#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <fstream>
# include <sstream>
# include <list>

# include "ServerConfig.hpp"
# include "parse_utils.hpp"

# define LOG_OPT	std::ios_base::out | std::ios_base::ate | std::ios_base::app

class HTTPConfig
{
private:
	//	Attributes
	std::stringstream		content;
	std::list<ServerConfig>	virtual_server_config;

public:
	// Member functions
	//		Constructor
	HTTPConfig(const std::string& confg_file);
	HTTPConfig(const HTTPConfig& other);
	//		Destructor
	~HTTPConfig();
	//Assignment operator
	HTTPConfig&	operator=(const HTTPConfig& other);
	//		Getters

private:
	//	parsing functions
	void	_parse();
	void	_parse_line(std::string& line);
	void	_parse_block(std::string& line);
	void	_insert_token(std::vector<std::string> tokens);
};

#endif