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
	// Constructor variables
	const std::string			_exec;
	const std::string			_root;
	const ServerConfig&			_conf;
	const Request&				_req;

	// Temporary variables
	std::string					_fullpath;
	std::vector<std::string>	_env;

	// execve variables
	char**						_envp;
	char*						_cmd[3];

public:
	string_map			cgi_headers;
	std::vector<char>	cgi_body;

private:
	void	_init_arrays();
	void	_run_cgi();
	void	_format_output(std::vector<char>& res_d);

public:
	CGI(const std::string& exec, const std::string& root, const ServerConfig& conf, const Request& req);
	~CGI();

	void	process();

};

#endif