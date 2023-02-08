#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <map>
#include <string>
#include <iostream>

#include "client/Client.hpp"
#include "server/Epoll.hpp"
#include "server/VirtualServer.hpp"
#include "config/HTTPConfig.hpp"

class HTTPServer
{
private:
	std::string			_config_file;
	HTTPConfig			_config;

	Epoll				_epoll;

	VirtualServer		_server; // tmp for tests

	Client				_client;

	// std::map<int, VirtualServer>	_virt_servers;
	// std::map<int, int>				_user_agents;

	HTTPServer();

	void	_parse_config();

	void	_add_server_to_epoll(const VirtualServer& server);

public:
	HTTPServer(const std::string& confg_file);
	~HTTPServer();

	void	run();

};

#endif
