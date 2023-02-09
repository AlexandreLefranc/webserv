#include "client/ClientManager.hpp"

void	ClientManager::create_client(int server_fd)
{
	int	client_fd;

	struct sockaddr_in	sockaddr;
	socklen_t			socklen;

	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen = sizeof(sockaddr);

	std::cout << YEL << "[ClientManager] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[ClientManager] OPENING fd: " << CRESET;
	client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	if (fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	std::cout << YEL << client_fd << CRESET << std::endl;

	_clients[client_fd] = new Client;

	return client_fd;
}