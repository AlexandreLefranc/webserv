#include "core/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& confg_file):
	_config_file(confg_file)
{
	std::cout << CYN << "[HTTPServer] Parsing configuration file" << CRESET << std::endl;
	std::cout << CYN << "[HTTPServer] Initiate epoll" << CRESET << std::endl;
	_epoll.init();

	std::cout << CYN << "[HTTPServer] Initiate virtual servers" << CRESET << std::endl;
	_server_manager.init();
	const std::vector<int>	serv_fds = _server_manager.getfds();
	for (std::vector<int>::const_iterator it = serv_fds.begin(); it != serv_fds.end(); it++)
	{
		_fds[*it] = SERVER;
		_epoll.add_fd(*it, EPOLLIN | EPOLLET);
	}
}

HTTPServer::~HTTPServer()
{}

void	HTTPServer::run()
{
	struct epoll_event	event[EPOLL_SIZE];
	int					nfds;
	std::cout << CYN << "[HTTPServer] Starting event loop" << CRESET << std::endl;
	while (true)
	{
		std::cout << CYN << "[HTTPServer] New event loop iteration" << CRESET << std::endl;
		_epoll.wait(event, nfds);

		std::cout << CYN << "[HTTPServer] Received " << nfds << " events" << CRESET << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			std::cout << CYN << "[HTTPServer] Event from fd " << event[i].data.fd << CRESET << std::endl;
			if (_fds[event[i].data.fd] == SERVER)
			{
				std::cout << CYN << "[HTTPServer] Client connection request!" << CRESET << std::endl;
				int client_fd = _client_manager.create_client(event[i].data.fd);
				_client_manager.add_client(client_fd, event[i].data.fd);
				// _client.accept_conection(event[i].data.fd);
				_fds[client_fd] = CLIENT;
				_epoll.add_fd(client_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
			}
			else
			{
				std::cout << CYN << "[HTTPServer] Client is talking!" << CRESET << std::endl;
				char	str[1024];
				if ((event[i].events & EPOLLRDHUP) != 0)
				{
					std::cout << CYN << "[HTTPServer] Client connection closed!" << CRESET << std::endl;
					_epoll.remove_fd(event[i].data.fd);
					_client.clear();
					_fds.erase(event[i].data.fd);
					break;
				}
				if ((event[i].events & EPOLLIN) != 0)
				{
					ssize_t ret = recv(event[i].data.fd, str, 1023, 0);
					str[ret] = '\0';
					std::cout << str;
					if ((event[i].events & EPOLLOUT) != 0)
					{
						send(event[i].data.fd, "GOT IT !\n", 10, 0);
					}
					// _epoll.remove_fd(event[i].data.fd);
					// _client.clear();
					// _fds.erase(event[i].data.fd);
				}
				if (std::strcmp(str, "STOP\r\n") == 0)
				{
					return;
				}
			}
		}

	}
}