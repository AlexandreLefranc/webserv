#include "HTTPConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

HTTPConfig::HTTPConfig(const std::string& config_file)
{
	std::ifstream	config(config_file.c_str());
	std::cout << RED << "[HTTPConfig] Initiate Config" << CRESET << std::endl;
	if (!config.is_open())
		throw (ParsingException());
	content << config.rdbuf();
	config.close();
	_parse();
}

HTTPConfig::HTTPConfig(const HTTPConfig& other)
	: content(other.content.str())
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

HTTPConfig::~HTTPConfig()
{
	return ;
}

/*==============================================================================
	Exception.
==============================================================================*/

HTTPConfig&	HTTPConfig::operator=(const HTTPConfig& other)
{
	if (this != &other)
	{
		content.str(other.content.str());
		virtual_server_config = other.virtual_server_config;
	}
	return (*this);
}

/*==============================================================================
	Getters.
==============================================================================*/

const std::list<ServerConfig>&	HTTPConfig::get_virtual_server_config() const
{
	return virtual_server_config;
}

/*==============================================================================

							PRIVATE MEMBER FUNCTIONS.

==============================================================================*/

/*==============================================================================
	Parsing functions.
==============================================================================*/

void	HTTPConfig::_parse()
{
	std::string		line;

	while (std::getline(content, line))
	{
		line = format_line(line);
		if (line.empty())
			continue ;
		_parse_line(line);
	}
}

void	HTTPConfig::_parse_block(std::string& line)
{
	if (line.find("server") != std::string::npos)
		virtual_server_config.push_back(ServerConfig(content));
	else if (line.find("http"))
	{
		std::getline(content, line);
		line = format_line(line);
		_parse_line(line);
	}
	else
		throw (ParsingException());
}

void	HTTPConfig::_parse_line(std::string& line)
{
	std::vector<std::string>	tokens;

	if (line.find("{") != std::string::npos)
		_parse_block(line);
	else
	{
		tokens = split_tokens(line);
		if (tokens.size() == 0)
			throw (ParsingException());
		//	Potential token check HERE.
		_insert_token(tokens);
	}
	return ;
}

void	HTTPConfig::_insert_token(std::vector<std::string> tokens)
{
	//	Other config options go HERE with `else if`.
	if (tokens.size() > 0)
		throw (ParsingException());
}