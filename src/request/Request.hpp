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
	t_http_method						_method;
	std::string							_target;
	std::map<std::string, std::string>	_arguments;
	std::string							_protocol;

	bool								_is_header_done;
	std::map<std::string, std::string>	_headers;

	bool								_has_body;
	std::string							_body_type;
	size_t								_body_len;
	std::string							_body;

private:
	std::string		_get_line();

	void	_process_start_line();

	bool	_process_header();
	void	_check_headers();

	bool	_process_body();
	
	t_http_method	_process_method(std::string method);

public:
	Request();
	~Request();

	void	set_client_fd(int client_fd);
	bool	parse_data(const std::string& str);

	//	Getters
	const std::string&	get_target() const;
	const std::string&	get_method() const;
};

#endif
