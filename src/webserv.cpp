#include <iostream>

#include "webserv.hpp"
#include "core/HTTPServer.hpp"

#include "request/HTMLGenerator.hpp"

int main(int argc, char** argv)
{
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

	// std::string root = "/mnt/nfs/homes/alefranc/Documents/cursus42/5_webserv/webserv";
	// std::string	target = "/";

	// std::cout << HTMLGenerator::dirlist(root+target);
	// std::cout << HTMLGenerator::error(404, "Not Found");



	return 0;
}