#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <fstream>
# include <iostream>

# include "ServerConfig.hpp"
# include "parse_utils.hpp"

# define NPOS	std::string::npos
# define LOG_OPT	std::ios_base::out | std::ios_base::ate | std::ios_base::app

class HTTPConfig
{
private:
	//	Attributes
	std::ifstream&			file;
	std::list<ServerConfig>	virtual_server_config;
	std::ofstream			log_stream;

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
	void	_add_location(std::string& line);
	void	_insert_entry(const std::vector<std::string> tokens);
	//	Utils functions
	void	_open_log_file(const std::string& log_file);
};

#endif