#include "client/Client.hpp"

Client::Client(int fd, int server_fd):
	fd(fd), server_fd(server_fd)
{}

Client::~Client()
{
	if (fd != -1)
	{
		std::cout << YEL << "[Client] CLOSING fd: " << fd << CRESET << std::endl;
		close(fd);
		fd = -1;
	}
}