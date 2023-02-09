#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <string>
# include <ios>
# include <fstream>

# include "ServerConfig.hpp"

# define NPOS	std::string::npos
# define LOG_OPT	std::ios_base::out | std::ios_base::ate | std::ios_base::app

class HTTPConfig
{
private:
	std::ifstream			_file;
	std::list<ServerConfig>	_virtual_server_config;
	// static const std::set<std::string>	_valid_keys;	

public:
	//	Attributes
	std::ofstream	log_stream;

	// Member functions
	//		Constructor
	HTTPConfig(const std::string& confg_file);
	//		Destructor
	~HTTPConfig();
	//		Getters

	//	Exception
	class ParsingException: public std::exception
	{
	public:
		const char*	what() const throw();
	};

private:
	//	parsing functions
	void	_parse();
	void	_parse_line(std::string& line);
	void	_parse_block(std::string& line);
	//	Utils functions
	bool						_is_key_valid(const std::string& key) const;
	const std::set<std::string>	_init_set();
};

#endif