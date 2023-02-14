#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include <map>
# include <string>
# include <iostream>

# include "webserv.hpp"
# include "client/Client.hpp"
# include "client/ClientManager.hpp"
# include "core/Epoll.hpp"
# include "server/ServerManager.hpp"
# include "server/VirtualServer.hpp"
# include "config/HTTPConfig.hpp"
# include "utils/Exceptions.hpp"

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

	std::string			_config_file;
	HTTPConfig			_config;

	Epoll				_epoll;

	ServerManager			_server_manager;
	ClientManager			_client_manager;

	// Client manip
	void	_create_client(int server_fd);
	void	_remove_client(int fd);
	void	_receive_client(int fd);

	int			_process_epollin(const epoll_event& event);


public:
	HTTPServer(const std::string& confg_file);
	~HTTPServer();

	void	run();

};

#endif
