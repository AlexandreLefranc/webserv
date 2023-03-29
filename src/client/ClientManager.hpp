#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

# include <map>

# include "client/Client.hpp"

// Purpose: Store all Clients in an organized and easy to access way.
// Perform action on Clients

class ClientManager
{
private: // Disable defaults behaviors
	ClientManager(const ClientManager& src);
	ClientManager&	operator=(const ClientManager& src);

private:
	std::map<int, Client*>	_clients;

public:
	ClientManager();
	~ClientManager();

	int		create_client(int server_fd, const HTTPConfig& httpconfig, const VirtualServer& virtualserver);
	void	remove_client(int client_fd);
	Client&	get_client(int client_fd);
};

#endif