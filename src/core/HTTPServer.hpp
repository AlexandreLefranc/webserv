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
private: // Disable defaults behaviors
	HTTPServer();
	HTTPServer(const HTTPServer& src);
	HTTPServer&	operator=(const HTTPServer& src);

private:

	std::map<int, bool>		_fds;

	// std::string			_config_file;
	HTTPConfig			_config;

	Epoll				_epoll;

	ServerManager			_server_manager;
	ClientManager			_client_manager;

	void	_create_client(int server_fd);
	bool	_is_client_disconnected(const epoll_event& event);

	std::string	_receive_all(int fd);

public:
	HTTPServer(const std::string& confg_file);
	~HTTPServer();

	void	run();

};

#endif
