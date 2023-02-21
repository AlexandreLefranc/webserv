#include "core/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& config_file)
	: _config(config_file)
	, _server_manager(_config)
{
	std::cout << CYN << "[HTTPServer] Initiate epoll" << CRESET << std::endl;
	_epoll.init();

	// std::cout << CYN << "[HTTPServer] Initiate virtual servers" << CRESET << std::endl;
	// _server_manager.init();
	const std::vector<int>	serv_fds = _server_manager.getfds();
	for (std::vector<int>::const_iterator it = serv_fds.begin(); it != serv_fds.end(); it++)
	{
		_fds[*it] = SERVER;
		_epoll.add_fd(*it, EPOLLIN | EPOLLET);
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
	int client_fd = _client_manager.create_client(server_fd);
	_fds[client_fd] = CLIENT;
	_epoll.add_fd(client_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
}

void	HTTPServer::_remove_client(int fd)
{
	std::cout << CYN << "[HTTPServer] Client connection closed!" << CRESET << std::endl;
	_epoll.remove_fd(fd);
	_client_manager.remove_client(fd);
	_fds.erase(fd);
}

void	HTTPServer::_receive_client(int fd)
{
	try
	{
		std::string	str = receive_all(fd); // can throw
		Request& request = _client_manager.get_client(fd).request;
		request.parse_data(str); // can throw
	}
	catch (const RecvException& e)
	{
		// Client disconnection
		_remove_client(fd);
	}
	catch (const CloseClientException& e)
	{
		_remove_client(fd);
	}
}

/*******************************************************************************
                                PRIVATE METHODS
*******************************************************************************/

void	HTTPServer::run()
{
	struct epoll_event	event[EPOLL_SIZE];
	int					nfds;
	std::cout << CYN << "[HTTPServer] Starting event loop" << CRESET << std::endl;
	while (true)
	{
		std::cout << CYN << "[HTTPServer] =========== New event loop iteration ===========" << CRESET << std::endl;
		_epoll.wait(event, nfds);

		std::cout << CYN << "[HTTPServer] Received " << nfds << " events" << CRESET << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			std::cout << CYN << "[HTTPServer] Event from fd " << event[i].data.fd << CRESET << std::endl;
			// display_epoll_event(event[i]);
			if (_fds[event[i].data.fd] == SERVER)
			{
				_create_client(event[i].data.fd);
			}
			else
			{
				std::cout << CYN << "[HTTPServer] Client is talking!" << CRESET << std::endl;
				if ((event[i].events & EPOLLRDHUP) != 0)
				{
					_remove_client(event[i].data.fd);
					continue;
				}
				if ((event[i].events & EPOLLIN) != 0)
				{
					_receive_client(event[i].data.fd);
				}
			}
		}

	}
}