#include <iostream>

#include "webserv.hpp"
#include "core/HTTPServer.hpp"

int main(int argc, char** argv)
{
	if (argc > 2)
	{
		std::cout << RED << "Usage: " << argv[0] << " [<configuration_file>]" << CRESET << std::endl;
		return 1;
	}

	std::string	confg_file("");
	if (argc == 2)
	{
		confg_file = argv[1];
	}

	try
	{
		HTTPServer	serv(confg_file);
		serv.run();
	}
	catch (const std::exception& e)
	{
		std::cout <<BRED<< "Server had to stop: " << e.what() << CRESET << std::endl;
		return 1;
	}

	return 0;
}