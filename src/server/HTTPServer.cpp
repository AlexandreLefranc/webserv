#include "server/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& confg_file):
	_config_file(confg_file)
{
	std::cout << CYN << "[HTTPServer] Parsing configuration file" << CRESET << std::endl;
	std::cout << CYN << "[HTTPServer] Initiate epoll" << CRESET << std::endl;
	_epoll.init();
	std::cout << CYN << "[HTTPServer] Initiate virtual servers" << CRESET << std::endl;
	_server_list.init();
	// _server.init();
	_add_servers_to_epoll();
	// _epoll.add_fd(_server.fd(), EPOLLIN | EPOLLET);
}

HTTPServer::~HTTPServer()
{}

void	HTTPServer::_add_servers_to_epoll()
{
	typedef	ServerList::server_list_t				outer_t;
	typedef	ServerList::server_list_t::mapped_type	inner_t;

	const outer_t&		server_list = _server_list.l();
	for (outer_t::const_iterator outit = server_list.begin(); outit != server_list.end(); outit++)
	{
		for (inner_t::const_iterator innerit = outit->second.begin(); innerit != outit->second.end(); innerit++)
		{
			_epoll.add_fd(innerit->second.fd(), EPOLLIN | EPOLLET);
		}
	}
}

void	HTTPServer::run()
{
	struct epoll_event	event;
	int					nfds;
	std::cout << CYN << "[HTTPServer] Starting event loop" << CRESET << std::endl;
	while (true)
	{
		std::cout << CYN << "[HTTPServer] New event loop iteration" << CRESET << std::endl;
		_epoll.wait(event, nfds);

		std::cout << CYN << "[HTTPServer] Received " << nfds << " events" << CRESET << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if (_server_list.is_fd_in_list(event.data.fd) == true)
			{
				std::cout << CYN << "[HTTPServer] Client connection request!" << CRESET << std::endl;
				_client.accept_conection(event.data.fd);
				_epoll.add_fd(_client.fd(), EPOLLIN | EPOLLET);
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