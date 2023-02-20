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
	typedef std::map<std::string, VirtualServer*>	inner_map_t;
	typedef	std::map<std::string, inner_map_t>		outer_map_t;
	typedef	outer_map_t								servers_t;

private:
	// map<ip:port, map<server_name, VirtualServer*> >
	servers_t				_servers;

	

public:
	ServerManager();
	~ServerManager();

	void	init();

	const std::vector<int>	getfds() const;
};

#endif