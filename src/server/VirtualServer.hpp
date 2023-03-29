#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>

# include <cstring>
# include <iostream>

# include "webserv.hpp"
# include "config/ServerConfig.hpp"

// Purpose: Keep all info relative to a specific VirtualServer.
// Manage the socket fd for this VirtualServer

struct VirtualServer
{
private: // Disable defaults behaviors
	VirtualServer();
	VirtualServer(const VirtualServer& src);
	VirtualServer&	operator=(const VirtualServer& src);

public:
	//	Typedefs
	typedef struct sockaddr_in	t_sockaddr_in;
	typedef struct sockaddr		t_sockaddr;

	//	Attributes
	int											fd;
	const ServerConfig&							default_config;
	std::map<std::string, const ServerConfig*>	configs;

	//	Constructor & Destructor
	VirtualServer(const ServerConfig& config);
	~VirtualServer();

	// Member function
	void				add_config(const ServerConfig& config);
	const ServerConfig&	get_config(const std::string& server_name) const;
};

#endif