#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <map>
#include <string>
#include <iostream>

#include "client/Client.hpp"
#include "server/Epoll.hpp"
#include "server/ServerList.hpp"
#include "server/VirtualServer.hpp"
#include "config/HTTPConfig.hpp"

class HTTPServer
{
private:
	std::string			_config_file;
	HTTPConfig			_config;

	Epoll				_epoll;

	// VirtualServer		_server; // tmp for tests

	Client				_client;

	ServerList			_server_list;
	// ClientList			_client_list;

	HTTPServer();

	void	_add_servers_to_epoll();

public:
	HTTPServer(const std::string& confg_file);
	~HTTPServer();

	void	run();

};

#endif
