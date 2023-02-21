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

void	HTTPServer::_create_client(int server_fd)
{
	std::cout << CYN << "[HTTPServer] Client connection request!" << CRESET << std::endl;
	int client_fd = _client_manager.create_client(server_fd);
	_fds[client_fd] = CLIENT;
	_epoll.add_fd(client_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
}

bool	HTTPServer::_is_client_disconnected(const epoll_event& event)
{
	if ((event.events & EPOLLRDHUP) != 0)
	{
		std::cout << CYN << "[HTTPServer] Client connection closed!" << CRESET << std::endl;
		_epoll.remove_fd(event.data.fd);
		_client_manager.remove_client(event.data.fd);
		_fds.erase(event.data.fd);
		return true;
	}
	return false;
}

std::string	HTTPServer::_receive_all(int fd)
{
	std::string	full("");
	char str[BUFF_SIZE];
	ssize_t ret;

	do
	{
		std::cout << "Reading socket: ";
		ret = recv(fd, str, BUFF_SIZE - 1, MSG_DONTWAIT);
		std::cout << ret << " bytes" << std::endl;
		if (ret > 0)
		{
			str[ret] = '\0';
			full += str;
		}
		if (ret == 0)
		{
			// Client disconnection during transmission
		}
	} while (ret > 0);

	return full;
}

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
			display_epoll_event(event[i]);
			if (_fds[event[i].data.fd] == SERVER)
			{
				_create_client(event[i].data.fd);
			}
			else
			{
				std::cout << CYN << "[HTTPServer] Client is talking!" << CRESET << std::endl;
				if (_is_client_disconnected(event[i]) == true)
				{
					continue;
				}
				if ((event[i].events & EPOLLIN) != 0)
				{
					std::string str = _receive_all(event[i].data.fd);
					std::cout << str;
					if ((event[i].events & EPOLLOUT) != 0)
					{
						send(event[i].data.fd, "GOT IT !\n", 10, 0);
					}
					// _epoll.remove_fd(event[i].data.fd);
					// _client_manager.remove_client(event[i].data.fd);
					// _fds.erase(event[i].data.fd);
					if (str == "STOP\r\n")
					{
						return;
					}
				}
			}
		}

	}
}