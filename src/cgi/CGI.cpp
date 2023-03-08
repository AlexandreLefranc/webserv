#include "cgi/CGI.hpp"

CGI::CGI(const std::string& exec, const std::string& root, const ServerConfig& conf, const Request& req)
	: _exec(exec), _root(root), _conf(conf), _req(req)
{
	std::cout << "Construct CGI" << std::endl;

	if (access(_exec.c_str(), X_OK) != 0)
	{
		throw std::runtime_error("CGI exec is not executable");
	}
}

CGI::~CGI()
{}

std::vector<char>	CGI::process()
{
	std::vector<std::string> env;
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("REQUEST_METHOD=" + _req._method);
	env.push_back("REQUEST_URI=" + _req._target);
	env.push_back("SCRIPT_FILENAME=" + _root + _req._target);
	env.push_back("DOCUMENT_ROOT=" + _root);
	env.push_back("REDIRECT_STATUS=301");

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
	env.push_back("QUERY_STRING=" + query_string);

	for (it = _req._headers.begin(); it != _req._headers.end(); ++it)
	{
		env.push_back("HTTP_" + toupperstr(it->first) + "=" + it->second);
	}

	display_vector(env, "env");

	char* envp[env.size() + 1];
	for (size_t i = 0; i < env.size(); ++i)
	{
		envp[i] = const_cast<char*>(env[i].c_str());
	}
	envp[env.size()] = NULL;

	display_cstyle_string_array(envp, "envp");

	std::string	fullpath = _root + _req._target;
	const char* cmd[] = {_exec.c_str(), fullpath.c_str(), NULL};

	display_cstyle_string_array(const_cast<char**>(cmd), "cmd");




	std::vector<char>	res_d;
	int					pipefd[2];
	if (pipe(pipefd) < 0)
	{
		throw std::runtime_error("pipe() failed");
	}

	pid_t	pid = fork();
	if (pid < 0)
	{
		throw std::runtime_error("fork() failed");
	}

	if (pid == 0)
	{
		close(pipefd[READ_END]);
		close(STDOUT_FILENO);
		dup2(pipefd[WRITE_END], STDOUT_FILENO);

		execve(cmd[0], const_cast<char* const*>(cmd), envp);
		std::cout << "execve error" << std::endl;
		std::cout << strerror(errno) << std::endl;
		exit(1);
	}
	else
	{
		close(pipefd[WRITE_END]);

		char	buffer[BUFF_SIZE];
		int		nbytes;

		while ((nbytes = read(pipefd[READ_END], buffer, BUFF_SIZE - 1)) != 0)
		{
			std::cout << nbytes << std::endl;
			res_d.insert(res_d.end(), buffer, buffer + nbytes);
		}
		wait(NULL);

		close(pipefd[READ_END]);
	}

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

	return res_d;
}
