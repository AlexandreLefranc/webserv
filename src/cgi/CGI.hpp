#ifndef CGI_HPP
# define CGI_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <unistd.h>

# include <errno.h>
# include <string.h>

# include <iostream>
# include <string>
# include <vector>

# include "config/ServerConfig.hpp"
# include "request/Request.hpp"

# define READ_END	0
# define WRITE_END	1

class CGI
{
private: // Disable defaults behaviors
	CGI();
	CGI(const CGI& src);
	CGI&	operator=(const CGI& src);

private:
	const std::string	_exec;
	const ServerConfig&	_conf;
	const Request&		_req;

public:
	std::map<std::string, std::string>	cgi_headers;
	std::vector<char>					cgi_body;

private:
	void	_init_arrays();
	void	_run_cgi();
	void	_format_output();

public:
	CGI(const std::string& exec, const ServerConfig& conf, const Request& req);
	~CGI();

	std::vector<char>	process();

};

#endif