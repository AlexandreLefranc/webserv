#include "HTTPConfig.hpp"

/*==============================================================================
	Constructors.
==============================================================================*/

// const std::set<std::string>	HTTPConfig::_valid_keys = _init_set();

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

HTTPConfig::HTTPConfig(const std::string& config_file)
	: _file(config_file)
	, log_stream( std::cout )
{
	std::cout << RED << "[HTTPConfig] Initiate Config" << CRESET << std::endl;
	_parse();
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

const char*	HTTPConfig::ParsingException::what() const throw()
{
	return ("HTTP parsing error.");
}

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

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
		if (line.find("{") == NPOS)
			_parse_line(line);
		else
			_parse_block(line); // throw (ParsingException());
	}
	_file.close();
}

void	_parse_block(const std::string& line)
{
	if (line.find("server") != NPOS)
		_virtual_server_config.push_back(ServerConfig(this->_file));
	else if (line.find("http"))
	{
		std::getline(_file, line);
		line = format_line(line);
		_parse_line(line);
	}
	else
		throw (ParsingException());
}

void	_parse_line(const std::string& line)
{
	std::pair<std::string, std::string>	entry;

	if (line.find("{") != NPOS)
		_parse_block(line);
	else
		_insert_entry(split_key_value(line));
}

void	_insert_entry(const std::pair<std::string, std::string>	entry)
{
	if (entry.first == "log_file")
	{
		_open_log_file(entry.second);
		if (!log_stream.is_open())
			throw (ParsingException());
	}
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