#include "client/ClientManager.hpp"

ClientManager::ClientManager()
{}

ClientManager::~ClientManager()
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		delete it->second;
	}
}

int	ClientManager::create_client(int server_fd)
{
	int	client_fd;

	struct sockaddr_in	sockaddr;
	socklen_t			socklen;

	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen = sizeof(sockaddr);

	std::cout << YEL << "[ClientManager] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[ClientManager] OPENING fd: " << CRESET;
	client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	if (client_fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	std::cout << YEL << client_fd << CRESET << std::endl;

	_clients[client_fd] = new Client(client_fd, server_fd);

	return client_fd;
}

void	ClientManager::remove_client(int client_fd)
{
	if (_clients.find(client_fd) != _clients.end())
	{
		delete _clients.at(client_fd);
		_clients.erase(client_fd);
	}
}