#include "HTTPConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

HTTPConfig::HTTPConfig(const std::string& config_file)
{
	std::cout << RED << "[HTTPConfig] Initiate Config" << CRESET << std::endl;
	std::ifstream		config(config_file.c_str());
	// std::stringstream	content;

	if (!config.is_open())
		throw (ParsingException());
	content << config.rdbuf();
	config.close();
	_parse();
	_check();
}

/*==============================================================================
	Destructors.
==============================================================================*/

HTTPConfig::~HTTPConfig()
{
	return ;
}

/*==============================================================================
	Getters.
==============================================================================*/

const std::list<ServerConfig>&	HTTPConfig::get_virtual_server_config() const
{
	return (virtual_server_config);
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

	// std::cout << content.str() << std::endl;

	while (std::getline(content, line))
	{
		std::cout << line << std::endl;
		line = format_line(line);
		std::cout << RED << "[HTTP]Config line: " << line << CRESET << std::endl;
		if (line.empty())
			continue ;
		_parse_line(line);
	}
}

void	HTTPConfig::_parse_block(std::string& line)
{
	if (line.find("server") != std::string::npos)
		virtual_server_config.push_back(ServerConfig(&content));
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
	if (tokens.size() == 2 && tokens[0] == "client_max_body_size")
	{
		client_max_body_size = std::atoi(tokens[1].c_str());
		std::cout << RED << "[HTTP]Got client max body size: " << client_max_body_size << CRESET << std::endl;
	}
	else
		throw (ParsingException());
}

void	HTTPConfig::_check() const
{
	std::list<ServerConfig>::const_iterator it;

	if (virtual_server_config.empty() == true)
	{
		std::cout << BRED << "Config check failed: No virtual server" << CRESET << std::endl;
		throw ParsingException();
	}
	for (it = virtual_server_config.begin(); it != virtual_server_config.end(); ++it)
		it->check();
}