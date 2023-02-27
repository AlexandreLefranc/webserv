#include "CGI.hpp"

CGI::CGI(const std::string& exec)
	: _executable(exec)
{}

CGI::~CGI()
{}



std::string	CGI::run_script(const std::string& script) const
{
	std::string res("");

	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		throw std::runtime_error("pipe() failed");
	}

	pid_t	pid = fork();
	if (pid == -1)
	{
		throw std::runtime_error("fork() failed");
	}
	
	if (pid == 0) // child
	{
		close(pipefd[READ_END]);

		dup2(pipefd[WRITE_END], STDOUT_FILENO);
		close(pipefd[WRITE_END]);

		const char *cmd[] 	= {_executable.c_str(), script.c_str(), NULL};
		execve(cmd[0], const_cast<char* const *>(cmd), NULL);
	}
	else // parent
	{
		close(pipefd[WRITE_END]);
		wait(NULL);

		char buffer[BUFF_SIZE];
		int nbytes;
		while ((nbytes = read(pipefd[READ_END], buffer, BUFF_SIZE - 1)) > 0)
		{
			buffer[nbytes] = '\0';
			res += buffer;
		}

		close(pipefd[READ_END]);
	}

	return res;
}