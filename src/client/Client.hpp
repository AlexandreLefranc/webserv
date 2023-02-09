#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

#include "webserv.hpp"

// Purpose: Keep all info relative to a Client
// Manage socket fd for this client

struct Client
{
private: // Disable defaults behaviors
	Client();
	Client(const Client& src);
	Client&	operator=(const Client& src);

public:
	int		fd;
	int		server_fd;

	Client(int fd, int server_fd);
	~Client();
};

#endif