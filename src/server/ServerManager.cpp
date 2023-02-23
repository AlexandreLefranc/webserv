#include "server/ServerManager.hpp"

ServerManager::ServerManager(const HTTPConfig& conf)
{
	std::list<ServerConfig>::const_iterator it = conf.get_virtual_server_config().begin();
	std::list<ServerConfig>::const_iterator end = conf.get_virtual_server_config().end();

	for ( ; it != end; it++)
	{
		_create_virtual_server(*it);
	}
}

ServerManager::~ServerManager()
{
	typedef	ServerManager::outer_map_t	outer_t;
	typedef	ServerManager::inner_map_t	inner_t;

	for (outer_t::iterator outerit = _servers.begin(); outerit != _servers.end(); outerit++)
	{
		for (inner_t::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); innerit++)
		{
			delete innerit->second;
		}
	}
}

void	ServerManager::_create_virtual_server(const ServerConfig& sconf)
{
	std::string		ip			= nbtostr(sconf.get_ip());
	std::string		port		= nbtostr(sconf.get_port());
	std::string		server_name	= sconf.get_server_name();

	if (_servers.find(ip+":"+port) == _servers.end())
	{
		std::cout << GRN << "[ServerManager] Creating Virtual Server: "
			<< ip << ":" << port << "|" << server_name << CRESET << std::endl;
		_servers[ip+":"+port][server_name] = new VirtualServer(sconf);
	}
	else if (_servers[ip+":"+port].find(server_name) == _servers[ip+":"+port].end())
	{
		std::cout << GRN << "[ServerManager] Creating Virtual Server: "
			<< ip << ":" << port << "|" << server_name << CRESET << std::endl;
		_servers[ip+":"+port][server_name] = new VirtualServer(sconf);
	}
	else
	{
		std::cout << GRN << "[ServerManager] Warning! Virtual Server: "
			<< ip << ":" << port << "|" << server_name
			<< "already exists" << CRESET << std::endl;
	}
}

const std::vector<int>				ServerManager::getfds() const
{
	typedef	ServerManager::outer_map_t	outer_t;
	typedef	ServerManager::inner_map_t	inner_t;

	std::vector<int>	fds;

	for (outer_t::const_iterator outerit = _servers.begin(); outerit != _servers.end(); outerit++)
	{
		for (inner_t::const_iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); innerit++)
		{
			fds.push_back(innerit->second->fd);
		}
	}
	return fds;
}