#include <iostream>

#include "HTTPServer.hpp"

int main(int argc, char** argv)
{
	if (argc > 2)
	{
		std::cout << "Usage: " << argv[0] << " <configuration_file>" << std::endl;
		return 1;
	}

	// try
	// {
	// 	HTTPServer	serv(argv[1]);
	// 	serv.init(); // socket epoll etc
	// 	serv.run(); // infinite loop
	// }
	// catch (const std::exception& e)
	// {
	// 	std::cout << "Server had to stop: " << e.what() << std::endl;
	// 	return 1;
	// }

	return 0;
}