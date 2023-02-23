#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <iostream>

#include "webserv.hpp"
#include "utils/Exceptions.hpp"

std::string	receive_all(int fd)
{
	std::string	full("");
	char str[BUFF_SIZE];
	ssize_t ret;

	do
	{
		// std::cout << "Reading socket: ";
		ret = recv(fd, str, BUFF_SIZE - 1, MSG_DONTWAIT);
		// std::cout << ret << " bytes" << std::endl;
		if (ret > 0)
		{
			str[ret] = '\0';
			full += str;
		}
		else if (ret == 0)
		{
			throw RecvException();
		}
		else if (ret == -1)
		{
			// std::cout << full;
			return full;
		}
	} while (ret > 0);

	return full;
}