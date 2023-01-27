#include <iostream>

#include "HTTPServer.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Please provide a configuration file" << std::endl;
		return 1;
	}

	try
	{
		HTTPServer	serv;
		serv.parse(argv[1]);
		serv.init(); // socket epoll etc
		serv.run(); // infinite loop
	}
	catch (const std::exception& e)
	{
		std::cout << "Server had to stop: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}