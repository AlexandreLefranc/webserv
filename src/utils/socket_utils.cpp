#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <iostream>

#include "webserv.hpp"
#include "utils/Exceptions.hpp"

std::vector<char>	receive_all(int fd)
{
	std::vector<char>	data;
	char buffer[BUFF_SIZE];
	ssize_t ret;

	do
	{
		// std::cout << "Reading socket: ";
		ret = recv(fd, buffer, BUFF_SIZE, MSG_DONTWAIT);
		// std::cout << ret << " bytes" << std::endl;
		if (ret > 0)
		{
			// buffer[ret] = '\0';
			data.insert(data.end(), buffer, buffer + ret);
		}
		else if (ret == 0)
		{
			throw RecvException();
		}
		else if (ret == -1)
		{
			return data;
		}
	} while (ret > 0);

	throw std::runtime_error("Should never throw this...");
}