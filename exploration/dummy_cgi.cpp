#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string>	_env;

char** 						_envp;
char*						_cmd[4];

static void	init_arrays()
{
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("REQUEST_METHOD=POST");
	_env.push_back("REQUEST_URI=");
	_env.push_back("SCRIPT_FILENAME=" + _root + _req._target);
	_env.push_back("DOCUMENT_ROOT=" + _root);
	_env.push_back("REDIRECT_STATUS=301");
}

int main()
{
	init_arrays();


	return 0;
}