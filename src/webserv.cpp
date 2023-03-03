
#include <csignal>
#include <iostream>

#include "webserv.hpp"
#include "core/HTTPServer.hpp"

bool	g_running = true;

void	sighandler(int)
{
	std::cout << BRED << std::endl << "Signal caugth" << CRESET << std::endl;
	g_running = false;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);

	if (argc > 2)
	{
		std::cout << RED << "Usage: " << argv[0] << " [<configuration_file>]" << CRESET << std::endl;
		return 1;
	}

	std::string	config_file("config/default.conf");
	if (argc == 2)
		config_file = argv[1];

	try
	{
		HTTPServer	serv(config_file);
		serv.run();
	}
	catch (const std::exception& e)
	{
		std::cout <<BRED<< "Server had to stop: " << e.what() << CRESET << std::endl;
		return 1;
	}

	return 0;
}