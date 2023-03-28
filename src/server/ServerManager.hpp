#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <iostream>
# include <sstream>
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

private:
	std::map<std::pair<int, int>, VirtualServer*>	_servers;

	void	_create_virtual_server(const ServerConfig& sconf);

public:
	ServerManager(const HTTPConfig& conf);
	~ServerManager();

	const std::vector<int>	getfds() const;
	const ServerConfig&		get_server_config(int fd, const std::string& server_name) const;
	const VirtualServer&	get_virtual_server(int fd) const;
};

#endif