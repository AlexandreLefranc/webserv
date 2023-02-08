#ifndef SERVERLIST_HPP
#define SERVERLIST_HPP

#include <iostream>
#include <string>
#include <map>

#include "VirtualServer.hpp"

class ServerList
{
public:
	typedef	std::map<std::string, std::map<std::string, VirtualServer> >	server_list_t;

private:
	// map<ip:port, map<server_name, VirtualServer> >
	server_list_t	_servers;

public:
	ServerList();
	~ServerList();

	void	init();

	const server_list_t&	l();
	bool					is_fd_in_list(int fd);
	const VirtualServer&	get_virtual_server(int fd);
};

#endif