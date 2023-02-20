#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const ServerConfig& config):
	fd(-1), config(config)
{
	std::cout << GRN << "[VirtualServer] Init" << CRESET << std::endl;
	std::cout << GRN << "[VirtualServer] OPENING fd: " << CRESET;
	fd = socket(config.family, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (fd == -1)
	{
		throw std::runtime_error("socket() failed");
	}
	std::cout << GRN << fd << CRESET << std::endl;

	struct sockaddr_in	  sockaddr;
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t			socklen = sizeof(struct sockaddr_in);
	sockaddr.sin_family			= config.family;
	sockaddr.sin_addr.s_addr	= htonl(config.addr);
	sockaddr.sin_port			= htons(config.port);

	if (bind(fd, reinterpret_cast<const struct sockaddr*>(&sockaddr), socklen) < 0)
	{
		close(fd);
		throw std::runtime_error("bind() failed");
	}

	if (listen(fd, 42) < 0)
	{
		close(fd);
		throw std::runtime_error("listen() failed");
	}
}

VirtualServer::~VirtualServer()
{
	if (fd != -1)
	{
		std::cout << GRN << "[VirtualServer] CLOSING fd: " << fd << CRESET << std::endl;
		close(fd);
	}
}