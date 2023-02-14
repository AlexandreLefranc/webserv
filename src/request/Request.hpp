#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <sys/socket.h>

# include <string>
# include <map>
# include <iostream>

# include "webserv.hpp"
# include "utils/Exceptions.hpp"

class Request
{
private: // Disable defaults behaviors
	Request(const Request& src);
	Request&	operator=(const Request& src);

private:
	int									_client_fd;

	std::string							_raw;

	bool								_has_start_line;
	std::string							_method;
	std::string							_route;
	std::string							_protocol;

	std::map<std::string, std::string>	_headers;

public:
	Request();
	~Request();

	void	set_client_fd(int client_fd);
	void	parse_data(const std::string& str);
};

#endif