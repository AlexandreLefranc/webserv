#include "server/ServerManager.hpp"

ServerManager::ServerManager(const HTTPConfig& conf)
{
	std::vector<ServerConfig>::const_iterator it = conf.get_virtual_server_config().begin();
	std::vector<ServerConfig>::const_iterator end = conf.get_virtual_server_config().end();

	if (it == end)
	{
		throw std::runtime_error("No virtual server");
	}

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
	std::stringstream ssip;
	ssip << sconf.get_ip();

	std::stringstream ssport;
	ssport << sconf.get_port();

	std::string		ip			= ssip.str();
	std::string		port		= ssport.str();
	std::string		server_name	= sconf.get_server_name();

	std::cout << ip+":"+port << std::endl;

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

const ServerConfig&	ServerManager::get_server_config(int fd) const
{
	typedef	ServerManager::outer_map_t	outer_t;
	typedef	ServerManager::inner_map_t	inner_t;

	for (outer_t::const_iterator outerit = _servers.begin(); outerit != _servers.end(); outerit++)
	{
		for (inner_t::const_iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); innerit++)
		{
			if (innerit->second->fd == fd)
			{
				return innerit->second->config;
			}
		}
	}

	throw std::runtime_error("Server not found");
}