#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include <map>
# include <string>
# include <iostream>

# include "client/Client.hpp"
# include "client/ClientManager.hpp"
# include "core/Epoll.hpp"
# include "server/ServerManager.hpp"
# include "server/VirtualServer.hpp"
# include "config/HTTPConfig.hpp"

// Purpose: Supervise relations between Epoll, Servers and Clients.
// Run main loop and decides what to do

class HTTPServer
{
private:

	std::map<int, bool>		_fds;

	std::string			_config_file;
	HTTPConfig			_config;

	Epoll				_epoll;

	Client				_client;

	ServerManager			_server_manager;
	ClientManager			_client_manager;

	HTTPServer();

public:
	HTTPServer(const std::string& confg_file);
	~HTTPServer();

	void	run();

};

#endif
