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
	size_t					client_max_body_size;

	//	Constructors
	HTTPConfig();
	HTTPConfig(const HTTPConfig& other);
	//Assignment operator
	HTTPConfig&	operator=(const HTTPConfig& other);
public:
	// Member functions
	//		Constructors
	HTTPConfig(const std::string& confg_file);
	//		Destructor
	~HTTPConfig();
	//		Getters
	const std::list<ServerConfig>&	get_virtual_server_config() const;
	size_t							get_max_body_size() const;

private:
	//	parsing functions
	void	_parse();
	void	_parse_line(std::string& line);
	void	_parse_block(std::string& line);
	void	_insert_token(std::vector<std::string> tokens);

	// check
	void	_check() const;
};

#endif