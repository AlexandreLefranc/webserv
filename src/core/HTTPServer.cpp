#include "core/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& config_file)
	: _config(config_file)
	, _epoll()
	, _server_manager(_config)
{
	const std::vector<int>	serv_fds = _server_manager.getfds();
	for (std::vector<int>::const_iterator it = serv_fds.begin(); it != serv_fds.end(); it++)
	{
		_fds[*it] = "SERVER";
		_epoll.add_fd(*it, EPOLLIN);
	}
}

HTTPServer::~HTTPServer()
{}

/*******************************************************************************
                                PRIVATE METHODS
*******************************************************************************/

void	HTTPServer::_create_client(int server_fd)
{
	std::cout << CYN << "[HTTPServer] Client connection request!" << CRESET << std::endl;

	const	ServerConfig&	config = _server_manager.get_server_config(server_fd);

	int client_fd = _client_manager.create_client(server_fd, config);
	_fds[client_fd] = "CLIENT";
	_epoll.add_fd(client_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
}


void	HTTPServer::_remove_client(int fd)
{
	std::cout << CYN << "[HTTPServer] Remove client!" << CRESET << std::endl;
	_epoll.remove_fd(fd);
	_client_manager.remove_client(fd);
	_fds.erase(fd);
}


int		HTTPServer::_communicate_with_client(const struct epoll_event& event)
{
	std::cout << CYN << "[HTTPServer] Client is communicating!" << CRESET << std::endl;

	int				client_fd = event.data.fd;
	Client&	client = _client_manager.get_client(client_fd);

	if ((event.events & EPOLLRDHUP) != 0)
	{
		_remove_client(client_fd);
		return -1;
	}

	if ((event.events & EPOLLIN) != 0 && client.request_complete == false)
	{
		std::cout << CYN << "[HTTPServer] Receiving data from client!" << CRESET << std::endl;
		try
		{
			client.parse_request();
		}
		catch (const RecvException& e)
		{
			// Client disconnection
			_remove_client(client_fd);
			return -1;
		}
		catch (const CloseClientException& e)
		{
			_remove_client(client_fd);
			return -1;
		}
	}

	if ((event.events & EPOLLOUT) != 0 && client.request_complete == true)
	{
		std::cout << CYN << "[HTTPServer] Sending data to client!" << CRESET << std::endl;
		client.create_response();
		std::cout << CYN << "[HTTPServer] Created response!" << CRESET << std::endl;
		// client.send_response();
		// // send_example_page(client_fd);
		// CGI	cgi("/usr/bin/php-cgi", client.config, client.request);
		// cgi.process();

		_remove_client(client_fd);
		return -1;
	}

	return 0;
}

/*******************************************************************************
                                PRIVATE METHODS
*******************************************************************************/

void	HTTPServer::run()
{
	struct epoll_event	event[EPOLL_SIZE];
	int					nfds;

	std::cout << CYN << "[HTTPServer] Starting event loop" << CRESET << std::endl;
	while (g_running == true)
	{
		std::cout << CYN << "[HTTPServer] =========== New event loop iteration ===========" << CRESET << std::endl;
		// display_map(_fds, "monitored_fd");
		_epoll.wait(event, nfds);

		for (int i = 0; i < nfds; i++)
		{
			std::cout << CYN << "[HTTPServer] Consume event from fd " << event[i].data.fd << CRESET << std::endl;
			// display_epoll_event(event[i]);
			if (_fds[event[i].data.fd] == "SERVER")
			{
				_create_client(event[i].data.fd);
				continue;
			}
			
			if (_fds[event[i].data.fd] == "CLIENT")
			{
				int ret = _communicate_with_client(event[i]);
				if (ret == -1)
				{
					continue;
				}
			}
		}
	}
}