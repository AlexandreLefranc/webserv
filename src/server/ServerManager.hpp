#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>

# include "server/VirtualServer.hpp"
# include "config/ServerConfig.hpp"

// Purpose: Store all VirtualServers in an organized and easy to access way
// Perform action on VirtualServers

class ServerManager
{
private: // Disable defaults behaviors
	ServerManager(const ServerManager& src);
	ServerManager&	operator=(const ServerManager& src);

public:
	typedef	std::map<std::string, std::map<std::string, VirtualServer*> >	server_list_t;

private:
	// map<ip:port, map<server_name, VirtualServer*> >
	server_list_t				_servers;

	

public:
	ServerManager();
	~ServerManager();

	void	init();

	const server_list_t&	l() const;
	const std::vector<int>	getfds() const;
	bool					is_fd_in_list(int fd) const;
	const VirtualServer&	get_virtual_server(int fd) const;
	int						accept_connection(int server_fd) const;
};

#endif