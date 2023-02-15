#include <iostream>
#include <exception>

#include "HTTPConfig.hpp"
#include "webserv.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2)
		return (1);
	try
	{
		HTTPConfig	config = HTTPConfig(argv[1]);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return (2);
	}
	(void)argv;
	return (0);
}