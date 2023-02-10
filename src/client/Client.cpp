#include "client/Client.hpp"

Client::Client(int server_fd):
	fd(-1), server_fd(server_fd)
{
	struct sockaddr_in	sockaddr;
	socklen_t			socklen;

	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen = sizeof(sockaddr);

	std::cout << YEL << "[Client] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[Client] OPENING fd: " << CRESET;
	fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	if (fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	std::cout << YEL << fd << CRESET << std::endl;
}

Client::~Client()
{
	if (fd != -1)
	{
		std::cout << YEL << "[Client] CLOSING fd: " << fd << CRESET << std::endl;
		close(fd);
		fd = -1;
	}
}