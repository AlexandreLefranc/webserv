#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <cstring>
#include <iostream>

#include "webserv.hpp"

class	VirtualServer
{
private:
	int			_fd;
	int			_family;
	int			_addr;
	int			_port;
	std::string	_server_name;

public:
	VirtualServer();
	~VirtualServer();

	void	init();
	// int	accept_client();

	int		fd() const;
};

#endif