#include "VirtualServer.hpp"

VirtualServer::VirtualServer():
	_fd(-1), _family(AF_INET), _addr(INADDR_ANY), _port(8080)
{}

VirtualServer::~VirtualServer()
{
	if (_fd != -1)
	{
		std::cout << GRN << "[VirtualServer] CLOSING fd: " << _fd << CRESET << std::endl;
		close(_fd);
	}
}

void	VirtualServer::init()
{
	std::cout << GRN << "[VirtualServer] Init" << CRESET << std::endl;
	std::cout << GRN << "[VirtualServer] OPENING fd: " << CRESET;
	_fd = socket(_family, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (_fd == -1)
	{
		throw std::runtime_error("socket() failed");
	}
	std::cout << GRN << _fd << CRESET << std::endl;

	struct sockaddr_in	  sockaddr;
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t			   socklen = sizeof(struct sockaddr_in);
	sockaddr.sin_family = _family;
	sockaddr.sin_addr.s_addr = htonl(_addr);
	sockaddr.sin_port	   = htons(_port);

	if (bind(_fd, reinterpret_cast<const struct sockaddr*>(&sockaddr), socklen) < 0)
	{
		throw std::runtime_error("bind() failed");
	}

	if (listen(_fd, 42) < 0)
	{
		throw std::runtime_error("listen() failed");
	}
}

// int VirtualServer::accept_client()
// {
//	 int client_fd;
//	 struct sockaddr_in  client_sockaddr;
//	 memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
//	 socklen_t			   client_socklen = sizeof(struct sockaddr_in);

//	 std::cout << GRN << "[VirtualServer] Accept client" << CRESET << std::endl;
//	 std::cout << YEL << "[Client] OPENING fd: " << CRESET;
//	 client_fd = accept(fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
//	 if (client_fd < 0)
//	 {
//		 throw std::runtime_error("accept() failed");
//	 }
//	 std::cout << YEL << client_fd << CRESET << std::endl;

//	 return client_fd;
// }

// Accessors
int		VirtualServer::fd() const
{
	return _fd;
}