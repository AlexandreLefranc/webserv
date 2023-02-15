#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <sys/socket.h>

# include <string>
# include <map>
# include <iostream>
# include <algorithm>

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
	std::string							_target;
	std::string							_protocol;

	bool								_is_header_done;
	std::map<std::string, std::string>	_headers;

	bool								_has_body;
	std::string							_body_type;
	size_t								_body_len;
	std::string							_body;

	bool								_ready_to_respond;

private:
	void	_parse_body(const std::string& str);

public:
	Request();
	~Request();

	void	set_client_fd(int client_fd);
	void	parse_data(const std::string& str);
};

#endif
