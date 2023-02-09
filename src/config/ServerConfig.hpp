#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

struct ServerConfig
{
	int			family;
	int			addr;
	int			port;
	std::string	server_name;
};

#endif