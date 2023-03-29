#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const ServerConfig& config):
	fd(-1), default_config(config)
{
	configs[config.get_server_name()] = &config;

	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, getprotobyname("tcp")->p_proto);
	if (fd == -1)
	{
		throw std::runtime_error("socket() failed");
	}

	std::cout << GRN << "[VirtualServer] Opening fd " << fd << CRESET << std::endl;

	const int enable1 = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable1, sizeof(int)) < 0)
	{
		close(fd);
		throw std::runtime_error("setsockopt1() failed");
	}

	const int enable2 = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &enable2, sizeof(int)) < 0)
	{
		close(fd);
		throw std::runtime_error("setsockopt2() failed");
	}

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

void	VirtualServer::add_config(const ServerConfig& config)
{
	std::string server_name = config.get_server_name();

	if (configs.count(server_name) != 0)
		throw std::runtime_error("add_config()");

	configs[server_name] = &config;
}

const ServerConfig&	VirtualServer::get_config(const std::string& server_name) const
{
	if (configs.count(server_name) != 0)
		return *(configs.at(server_name));
	else
		return default_config;
}