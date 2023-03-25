#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# include <cstring>
# include <iostream>

# include "webserv.hpp"
# include "request/Request.hpp"
# include "request/Response.hpp"
# include "config/HTTPConfig.hpp"
# include "config/ServerConfig.hpp"

// Purpose: Keep all info relative to a Client
// Manage socket fd for this client

struct Client
{
private: // Disable defaults behaviors
	Client();
	Client(const Client& src);
	Client&	operator=(const Client& src);

public:
	int						fd;
	const HTTPConfig&		httpconfig;
	const ServerConfig&		config;

	Request		request;
	bool		request_complete;
	
	Response	response;

	Client(int server_fd, const HTTPConfig& httpconfig, const ServerConfig& config);
	~Client();

	void	parse_request();
	void	create_response();
	void	send_response();
};

#endif