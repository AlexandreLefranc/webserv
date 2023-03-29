#include "server/ServerManager.hpp"

ServerManager::ServerManager(const HTTPConfig& conf)
{
	if (conf.get_virtual_server_config().size() == 0)
		throw std::runtime_error("No virtual server");

	std::vector<ServerConfig>::const_iterator it;
	for (it = conf.get_virtual_server_config().begin(); it != conf.get_virtual_server_config().end(); it++)
		_create_virtual_server(*it);


	// Debug code
	// std::map<std::pair<int, int>, VirtualServer*>::const_iterator it2;
	// for (it2 = _servers.begin(); it2 != _servers.end(); ++it2)
	// {
	// 	std::cout << it2->first.first << ":" << it2->first.second << std::endl;
	// 	std::map<std::string, const ServerConfig*> configs = it2->second->configs;

	// 	std::cout << "  default conf = " << it2->second->default_config.get_server_name() << std::endl;

	// 	std::map<std::string, const ServerConfig*>::const_iterator	it_conf;
	// 	for (it_conf = configs.begin(); it_conf != configs.end(); ++it_conf)
	// 	{
	// 		std::cout << "  " << it_conf->second->get_server_name() << std::endl;
	// 	}
	// }
	// End of debug code
}

ServerManager::~ServerManager()
{
	std::map<std::pair<int, int>, VirtualServer*>::iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		delete it->second;
	}
}

void	ServerManager::_create_virtual_server(const ServerConfig& sconf)
{
	int			ip = sconf.get_ip();
	int			port = sconf.get_port();
	std::pair<int, int> ip_port(ip, port);

	std::string	server_name	= sconf.get_server_name();
	std::cout << ip << ":" << port << "|" << server_name << std::endl;

	if (_servers.find(ip_port) == _servers.end())
	{
		std::cout << GRN << "[ServerManager] Creating Virtual Server: "
			<< ip << ":" << port << "|" << server_name << CRESET << std::endl;
		_servers[ip_port] = new VirtualServer(sconf);
	}
	else
	{
		std::cout << GRN << "[ServerManager] Appending Virtual Server: "
			<< ip << ":" << port << "|" << server_name << CRESET << std::endl;
		_servers[ip_port]->add_config(sconf);
	}
}

const std::vector<int>				ServerManager::getfds() const
{
	std::vector<int>	fds;

	std::map<std::pair<int, int>, VirtualServer*>::const_iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		fds.push_back(it->second->fd);
	}

	return fds;
}

const ServerConfig&	ServerManager::get_server_config(int fd, const std::string& server_name) const
{
	std::map<std::pair<int, int>, VirtualServer*>::const_iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (it->second->fd == fd)
			return it->second->get_config(server_name);
	}

	throw std::runtime_error("Server not found");
}

const VirtualServer&	ServerManager::get_virtual_server(int fd) const
{
	std::map<std::pair<int, int>, VirtualServer*>::const_iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (it->second->fd == fd)
			return *(it->second);
	}

	throw std::runtime_error("Virtual Server not found");
}