#include "ServerConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

ServerConfig::ServerConfig(std::ifstream& config)
	: file(config)
	, log_stream(std::cout)
{
	std::cout << RED << "[ServerConfig] Initiate Config" << CRESET << std::endl;
	_parse();
}

ServerConfig::ServerConfig(const ServerConfig& other)
	: file(other.file)
	, log_stream(other.log_stream)
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

ServerConfig::~ServerConfig()
{
	if (log_stream.is_open())
		log_stream.close();
	return ;
}

/*==============================================================================
	Assignment operator.
==============================================================================*/

ServerConfig&	ServerConfig::operator=(const ServerConfig& other)
{
	if (this != &other)
	{
		file = other.file;
		log_stream = other.log_stream;
	}
}

/*==============================================================================

							PRIVATE MEMBER FUNCTIONS.

==============================================================================*/

/*==============================================================================
	Parsing functions.
==============================================================================*/

void	ServerConfig::_parse()
{
	std::string		line;

	if (!file.is_open())
		throw (std::ios_base::failure("Config file error."));
	while (std::getline(file, line))
	{
		line = format_line(line);
		if (line.empty())
			continue ;
		else if (line == "}")
			return ;
		_parse_line(line);
	}
	file.close();
}

void	ServerConfig::_parse_line(std::string& line)
{
	const std::vector<std::string>	tokens;

	if (line.find("{") != NPOS)
	{
		tokens = split_tokens(line);
		if (tokens.front() == "location")
			_add_location(tokens);
		else
			throw (ParsingException());
	}
	else if (line == "}")
			return ;
	else
	{
		tokens = split_tokens(line);
		//	Potential token check HERE.
		_insert_token(tokens);
	}
	return ;
}

void	_insert_token(std::vector<std::string> tokens)
{
	if (tokens.front() == "root" && tokens.size() == 2)
		root = tokens[1];
	else if (tokens.front() == "server_name" && tokens.size() == 2)
		server_name = tokens[1];
	else if (tokens.front() == "listen" && tokens.size() == 2)
		
	else if (tokens.front() == "index")
		index = std::vector(tokens.begin() + 1, tokens.end());
	else
		throw (ParsingException());
}

void	_add_location(std::string& line)
{
	
}