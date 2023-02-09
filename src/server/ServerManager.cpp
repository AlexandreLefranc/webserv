#include "server/ServerManager.hpp"

ServerManager::ServerManager()
{}

ServerManager::~ServerManager()
{
	typedef	ServerManager::server_list_t				outer_t;
	typedef	ServerManager::server_list_t::mapped_type	inner_t;

	for (outer_t::iterator outerit = _servers.begin(); outerit != _servers.end(); outerit++)
	{
		for (inner_t::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); innerit++)
		{
			delete innerit->second;
		}
	}
}

void	ServerManager::init()
{
	std::string	ip = "127.0.0.1";
	std::string	port = "8080";
	std::string	server_name = "default";

	int n_vserver = 1;
	for (int i = 0; i < n_vserver; i++)
	{
		if (_servers.find(ip+":"+port) == _servers.end())
		{
			std::cout << GRN << "[ServerManager] No virtual server for " << ip+":"+port << CRESET << std::endl;

			ServerConfig conf = ServerConfig(); // tmp
			conf.family = AF_INET; // tmp
			conf.addr = INADDR_ANY; // tmp
			conf.port = 8080; // tmp
			conf.server_name = "default"; // tmp

			_servers[ip+":"+port][server_name] = new VirtualServer(conf);
		}
		else
		{
			std::cout << GRN << "[ServerManager] virtual server found for " << ip+":"+port << CRESET << std::endl;
		}
	}
}

const ServerManager::server_list_t&	ServerManager::l() const
{
	return _servers;
}

const std::vector<int>				ServerManager::getfds() const
{
	typedef	ServerManager::server_list_t				outer_t;
	typedef	ServerManager::server_list_t::mapped_type	inner_t;

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


bool					ServerManager::is_fd_in_list(int fd) const
{
	typedef	ServerManager::server_list_t				outer_t;
	typedef	ServerManager::server_list_t::mapped_type	inner_t;

	for (outer_t::const_iterator outerit = _servers.begin(); outerit != _servers.end(); outerit++)
	{
		for (inner_t::const_iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); innerit++)
		{
			if (fd == innerit->second->fd)
				return true;
		}
	}
	return false;
}

int						accept_connection(int server_fd) const
{
	int					fd;
	struct sockaddr_in	sockaddr;
	socklen_t			socklen;

	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen = sizeof(sockaddr);

	std::cout << GRN << "[ServerManager] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[Client] OPENING fd: " << CRESET;
	fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	if (fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	std::cout << YEL << fd << CRESET << std::endl;

	return fd;
}