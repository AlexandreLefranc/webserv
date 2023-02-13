#include "HTTPConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

HTTPConfig::HTTPConfig(const std::string& config_file)
	: _file(ifstream(config_file))
	, log_stream(std::cout)
{
	std::cout << RED << "[HTTPConfig] Initiate Config" << CRESET << std::endl;
	_parse();
}

HTTPConfig::HTTPConfig(const HTTPConfig& other)
	: _file(other._file)
	, log_stream(other.log_stream)
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

HTTPConfig::~HTTPConfig()
{
	if (log_stream.is_open())
		log_stream.close();
	if (file.is_open())
		file.close();
	return ;
}

/*==============================================================================
	Exception.
==============================================================================*/

HTTPConfig&	HTTPConfig::operator=(const HTTPConfig& other)
{
	if (this != &other)
	{
		_file = other._file;
		log_stream = other.log_stream;
	}
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

	if (!_file.is_open())
		throw (std::ios_base::failure("Config file error."));
	while (std::getline(_file, line))
	{
		line = format_line(line);
		if (line.empty())
			continue ;
			_parse_line(line);
	}
	_file.close();
}

void	HTTPConfig::_parse_block(std::string& line)
{
	if (line.find("server") != NPOS)
		_virtual_server_config.push_back(ServerConfig(_file));
	else if (line.find("http"))
	{
		std::getline(_file, line);
		line = format_line(line);
		_parse_line(line);
	}
	else
		throw (ParsingException());
}

void	HTTPConfig::_parse_line(std::string& line)
{
	const std::vector<std::string>	tokens;

	if (line.find("{") != NPOS)
		_parse_block(line);
	else
	{
		tokens = split_tokens(line);
		//	Potential token check HERE.
		_insert_token(tokens);
	}
	return ;
}

void	HTTPConfig::_insert_token(const std::vector<std::string> tokens)
{
	if (tokens.front() == "log_file")
	{
		_open_log_file(tokens.last());
		if (!log_stream.is_open())
			throw (ParsingException());
	}
	//	Other config options go HERE with `else if`.
	else
		throw (ParsingException());
}

/*==============================================================================
	Utils functions.
==============================================================================*/

void	HTTPConfig::_open_log_file(const std::string& log_file)
{
	log_stream.close();
	log_stream.open(log_file, LOG_OPT);
}