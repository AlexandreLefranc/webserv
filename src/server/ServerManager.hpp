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
	//	Typedefs
	typedef std::map<std::string, VirtualServer*>	inner_map_t;
	typedef	std::map<std::string, inner_map_t>		outer_map_t;
	typedef	outer_map_t								servers_t;

private:
	// map<ip:port, map<server_name, VirtualServer*> >
	servers_t				_servers;

	void	_create_virtual_server(const ServerConfig& sconf);

public:
	ServerManager(const HTTPConfig& conf);
	~ServerManager();

	const std::vector<int>	getfds() const;
	const ServerConfig&		get_server_config(int fd) const;
};

#endif