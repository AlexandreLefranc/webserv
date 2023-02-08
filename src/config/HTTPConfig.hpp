#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <string>
# include <ios>

class HTTPConfig
{
private:
	int							_log_file_fd;
	std::vector<ServerConfig>	_virtual_server_config;

public:
	HTTPConfig(const std::string& confg_file);
	~HTTPConfig();

private:
	_parse(std::stringstream& content);
};

#endif