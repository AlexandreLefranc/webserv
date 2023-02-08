#include "client/Client.hpp"

Client::Client():
	_fd(-1), _associated_server_fd(-1)
{
	memset(&_sockaddr, 0, sizeof(struct sockaddr_in));
	_socklen = sizeof(struct sockaddr_in);
}

Client::~Client()
{
	clear();
}

void	Client::accept_conection(int server_fd)
{
	std::cout << YEL << "[Client] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[Client] OPENING fd: " << CRESET;
	_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&_sockaddr), &_socklen);
	if (_fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	std::cout << YEL << _fd << CRESET << std::endl;

	_associated_server_fd = server_fd;
}

void	Client::clear()
{
	if (_fd != -1)
	{
		std::cout << YEL << "[Client] CLOSING fd: " << _fd << CRESET << std::endl;
		close(_fd);
		_fd = -1;
	}
	_associated_server_fd = -1;
}

int		Client::fd() const
{
	return _fd;
}