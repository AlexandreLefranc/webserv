#include "ServerConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

ServerConfig::ServerConfig(const std::string& config_file)
	: _file(ifstream(config_file))
	, log_stream( std::cout )
{
	std::cout << RED << "[ServerConfig] Initiate Config" << CRESET << std::endl;
	_parse();
}

ServerConfig::ServerConfig(const ServerConfig& other)
	: _file(other._file)
	, log_stream(other.log_stream)
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

ServerConfig::~ServerConfig()
{
	log_stream.close();
	return ;
}

/*==============================================================================
	Exception.
==============================================================================*/

ServerConfig&	ServerConfig::operator=(const ServerConfig& other)
{
	if (this != &other)
	{
		_file = other._file;
		log_stream = other.log_stream;
	}
}

/*==============================================================================
	Exception.
==============================================================================*/

const char*	ServerConfig::ParsingException::what() const throw()
{
	return ("HTTP parsing error.");
}
