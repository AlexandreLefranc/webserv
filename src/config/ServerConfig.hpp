#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>

class ServerConfig
{

public:
	//	Attributes
	std::ofstream	log_stream;

	// Member functions
	//		Constructor
	HTTPConfig(const std::string& confg_file);
	HTTPConfig(const HTTPConfig& other);
	//		Destructor
	~HTTPConfig();
	//Assignment operator
	HTTPConfig&	operator=(const HTTPConfig& other);
};
