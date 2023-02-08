#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

#include "webserv.hpp"

class Client
{
private:
	int		_fd;
	int		_associated_server_fd;

	struct sockaddr_in	_sockaddr;
	socklen_t			_socklen;

public:
	Client();
	~Client();

	void	accept_conection(int server_fd);
	void	clear();

	int		fd() const;
};

#endif