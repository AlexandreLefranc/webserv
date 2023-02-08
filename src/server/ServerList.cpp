#include "server/ServerList.hpp"

ServerList::ServerList()
{}

ServerList::~ServerList()
{}

void	ServerList::init()
{
	std::string	ip = "127.0.0.1";
	std::string	port = "8080";
	std::string	server_name = "default";

	int n_vserver = 1;
	for (int i = 0; i < n_vserver; i++)
	{
		if (_servers.find(ip+":"+port) == _servers.end())
		{
			std::cout << GRN << "[ServerList] No virtual server for " << ip+":"+port << CRESET << std::endl;
			_servers[ip+":"+port][server_name].init();
		}
		else
		{
			std::cout << GRN << "[ServerList] virtual server found for " << ip+":"+port << CRESET << std::endl;
		}
	}
}

const ServerList::server_list_t&	ServerList::l()
{
	return _servers;
}

bool					ServerList::is_fd_in_list(int fd)
{
	typedef	ServerList::server_list_t				outer_t;
	typedef	ServerList::server_list_t::mapped_type	inner_t;

	// const outer_t&		server_list = _server_list.l();
	for (outer_t::const_iterator outit = _servers.begin(); outit != _servers.end(); outit++)
	{
		for (inner_t::const_iterator innerit = outit->second.begin(); innerit != outit->second.end(); innerit++)
		{
			if (fd == innerit->second.fd())
				return true;
		}
	}
	return false;
}

// const VirtualServer&	ServerList::get_virtual_server(int fd)
// {

// }