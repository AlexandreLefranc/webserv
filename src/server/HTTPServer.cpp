#include "server/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& confg_file):
	_config_file(confg_file)
	, _config(HTTPConfig(_config_file))
{
	std::cout << CYN << "[HTTPServer] Initiate epoll" << CRESET << std::endl;
	_epoll.init();
	std::cout << CYN << "[HTTPServer] Initiate virtual servers" << CRESET << std::endl;
	_server.init();
	_epoll.add_fd(_server.fd, EPOLLIN | EPOLLET);
}

HTTPServer::~HTTPServer()
{}

void	HTTPServer::_parse_config()
{
	std::cout << "Parsing config..." << std::endl;
}

void	HTTPServer::run()
{
	struct epoll_event  event;
	int				 nfds;
	std::cout << CYN << "[HTTPServer] Starting event loop" << CRESET << std::endl;
	while (true)
	{
		_epoll.wait(event, nfds);

		std::cout << CYN << "[HTTPServer] Received " << nfds << " events" << CRESET << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if (event.data.fd == _server.fd)
			{
				std::cout << CYN << "[HTTPServer] Client connection request!" << CRESET << std::endl;
				_client.accept_conection(_server.fd);
				_epoll.add_fd(_client.fd, EPOLLIN | EPOLLET);
			}
			else
			{
				std::cout << CYN << "[HTTPServer] Client is talking!" << CRESET << std::endl;
				if ((event.events & EPOLLIN) != 0)
				{
					char	str[1024];
					ssize_t ret = recv(event.data.fd, str, 1023, 0);
					str[ret] = '\0';
					std::cout << str;
				}
				send(event.data.fd, "GOT IT !\n", 10, MSG_NOSIGNAL);
				_epoll.remove_fd(event.data.fd);
				_client.clear();
			}
		}

	}
}