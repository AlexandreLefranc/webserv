#include "HTTPConfig.hpp"

HTTPConfig::HTTPConfig(const std::string& config_file)
{
	std::cout << RED << "[HTTPConfig] Initiate Config" << CRESET << std::endl;
	_parse(config_file);
}

HTTPConfig::~HTTPConfig()
{
	return ;
}

HTTPConfig::_parse(std::string& file_name)
{
	std::ifstream	_file(file_name);
	std::string		line;

	if (!_file.is_open())
		throw (std::ios_base::failure("Config file error."));
	while (getline(_file, line))
	{
		line = format_line(line);
		if (line.empty())
			continue ;
		if (line.find("{") < 0)
			parse_line(line);
		else
			parse_block(line, _file);
	}
	_file.close();
}