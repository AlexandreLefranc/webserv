#include "client/ClientManager.hpp"

ClientManager::ClientManager()
{}

ClientManager::~ClientManager()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		delete it->second;
	}
}

int	ClientManager::create_client(int server_fd, const ServerConfig& config)
{
	Client* new_client = new Client(server_fd, config); // can throw
	_clients[new_client->fd] = new_client;

	return new_client->fd;
}

void	ClientManager::remove_client(int client_fd)
{
	if (_clients.find(client_fd) != _clients.end())
	{
		delete _clients.at(client_fd);
		_clients.erase(client_fd);
	}
}

Client&	ClientManager::get_client(int client_fd)
{
	return *_clients[client_fd];
}
