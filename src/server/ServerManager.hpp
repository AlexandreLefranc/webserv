#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <stdlib.h>

# include "config/HTTPConfig.hpp"
# include "config/ServerConfig.hpp"
# include "server/VirtualServer.hpp"

# include "webserv.hpp"

// Purpose: Store all VirtualServers in an organized and easy to access way
// Perform action on VirtualServers

class ServerManager
{
private: // Disable defaults behaviors
	ServerManager();
	ServerManager(const ServerManager& src);
	ServerManager&	operator=(const ServerManager& src);

public:
	typedef	std::map<std::string, std::map<std::string, VirtualServer*> >	server_list_t;

private:
	// map<ip:port, map<server_name, VirtualServer*> >
	server_list_t				_servers;

	void	_create_virtual_server(const ServerConfig& sconf);

public:
	ServerManager(const HTTPConfig& conf);
	~ServerManager();

	// void	init();

	const server_list_t&	l() const;
	const std::vector<int>	getfds() const;
	bool					is_fd_in_list(int fd) const;
	const VirtualServer&	get_virtual_server(int fd) const;
};

#endif