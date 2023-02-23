#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const ServerConfig& config):
	fd(-1), config(config)
{
	std::cout << GRN << "[VirtualServer] OPENING fd: " << CRESET;
	fd = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (fd == -1)
	{
		throw std::runtime_error("socket() failed");
	}
	std::cout << GRN << fd << CRESET << std::endl;

	t_sockaddr_in	  sockaddr;
	memset(&sockaddr, 0, sizeof(t_sockaddr_in));
	socklen_t			socklen = sizeof(t_sockaddr_in);
	sockaddr.sin_family			= AF_INET;
	sockaddr.sin_addr.s_addr	= htonl(config.get_ip());
	sockaddr.sin_port			= htons(config.get_port());

	if (bind(fd, reinterpret_cast<const t_sockaddr*>(&sockaddr), socklen) < 0)
	{
		close(fd);
		throw std::runtime_error("bind() failed");
	}

	if (listen(fd, 42) < 0)
	{
		close(fd);
		throw std::runtime_error("listen() failed");
	}
	std::cout << GRN << "[VirtualServer] Listening on " << config.get_ip() << ":" << config.get_port() << CRESET << std::endl;
}

VirtualServer::~VirtualServer()
{
	if (fd != -1)
	{
		std::cout << GRN << "[VirtualServer] CLOSING fd: " << fd << CRESET << std::endl;
		close(fd);
	}
}