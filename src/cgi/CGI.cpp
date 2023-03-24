#include "cgi/CGI.hpp"

CGI::CGI(const std::string& exec, const std::string& root, const ServerConfig& conf, const Request& req)
	: _exec(exec), _root(root), _conf(conf), _req(req), _envp(NULL)
{
	if (access(_exec.c_str(), X_OK) != 0)
	{
		throw std::runtime_error("CGI exec is not executable");
	}
}

CGI::~CGI()
{
	if (_envp != NULL)
	{
		delete [] _envp;
	}
}

void				CGI::_init_arrays()
{
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("REQUEST_METHOD=" + _req._method);
	_env.push_back("REQUEST_URI=" + _req._target);
	_env.push_back("SCRIPT_FILENAME=" + _root + _req._target);
	_env.push_back("DOCUMENT_ROOT=" + _root);
	_env.push_back("REDIRECT_STATUS=301");

	std::string	query_string("");

	std::map<std::string, std::string>::const_iterator it;
	for (it = _req._target_param.begin(); it != _req._target_param.end(); ++it)
	{
		query_string += it->first + "=" + it->second + "&";
	}

	if (query_string.length() > 0)
	{
		query_string.erase(query_string.length() - 1);
	}

	std::cout << "query_string=" << query_string << std::endl;
	_env.push_back("QUERY_STRING=" + query_string);

	for (it = _req._headers.begin(); it != _req._headers.end(); ++it)
	{
		std::string	key_upper = toupperstr(it->first);
		std::replace(key_upper.begin(), key_upper.end(), '-', '_');
		_env.push_back(key_upper + "=" + it->second);
	}

	display_vector(_env, "_env");

	_envp = new	char*[_env.size() + 1];
	for (size_t i = 0; i < _env.size(); ++i)
	{
		_envp[i] = const_cast<char*>(_env[i].c_str());
	}
	_envp[_env.size()] = NULL;

	display_cstyle_string_array(_envp, "_envp");

	_fullpath = _root + _req._target;
	_cmd[0] = const_cast<char*>(_exec.c_str());
	_cmd[1] = const_cast<char*>(_fullpath.c_str());
	_cmd[2] = NULL;

	display_cstyle_string_array(_cmd, "_cmd");
}


void				CGI::_run_cgi()
{
	std::vector<char>	res_d;
	int					pipein[2];
	int					pipeout[2];

	if (pipe(pipeout) < 0 || pipe(pipein) < 0)
	{
		throw std::runtime_error("pipe() failed");
	}

	std::cout << "Writing " << _req._body.size() << " bytes to execve" << std::endl;
	write(pipein[WRITE_END], _req._body.data(), _req._body.size());
	close(pipein[WRITE_END]);

	pid_t	pid = fork();
	if (pid < 0)
	{
		throw std::runtime_error("fork() failed");
	}

	if (pid == 0)
	{
		close(pipeout[READ_END]);
		close(STDOUT_FILENO);
		dup2(pipeout[WRITE_END], STDOUT_FILENO);

		close(STDIN_FILENO);
		dup2(pipein[READ_END], STDIN_FILENO);

		// std::ofstream	debug_out("debug_out");
		// char	buf[_req._body.size() + 1];
		// read(0, buf, _req._body.size());
		// buf[_req._body.size()] = '\0';
		// debug_out << buf;
		// debug_out.close();
		execve(_cmd[0], const_cast<char* const*>(_cmd), _envp);
		std::cerr << "execve error" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
	else
	{
		close(pipeout[WRITE_END]);
		close(pipein[READ_END]);

		char	buffer[BUFF_SIZE];
		int		nbytes;
		while ((nbytes = read(pipeout[READ_END], buffer, BUFF_SIZE - 1)) != 0)
		{
			std::cout << nbytes << std::endl;
			res_d.insert(res_d.end(), buffer, buffer + nbytes);
		}
		wait(NULL);

		close(pipeout[READ_END]);
	}

	_format_output(res_d);
}

void				CGI::_format_output(std::vector<char>& res_d)
{
	std::string res_s(res_d.begin(), res_d.end());
	// std::cout << res_s << std::endl;

	while (res_s.find("\r\n") != std::string::npos)
	{
		std::string	line = res_s.substr(0, res_s.find("\r\n"));
		res_s.erase(res_s.begin(), res_s.begin() + line.length() + 2);
		res_d.erase(res_d.begin(), res_d.begin() + line.length() + 2);

		if (line == "")
		{
			break;
		}

		std::vector<std::string>	splitted = split_first(line, ":");
		std::string					key = tolowerstr(trim(splitted[0]));
		std::string					value = trim(splitted[1]);
		cgi_headers[key] = value;
	}

	cgi_body = res_d;

	display_map(cgi_headers, "cgi_headers");

	std::cout << "body:" << std::endl;
	std::cout << std::string(cgi_body.begin(), cgi_body.end()) << std::endl;
}


void		CGI::process()
{
	_init_arrays();
	_run_cgi();	
}
