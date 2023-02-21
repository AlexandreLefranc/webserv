#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <algorithm>
# include <sstream>

# include "ansi_colors.hpp"

# define EPOLL_SIZE	200
# define BUFF_SIZE	32

enum	socket_type
{
	SERVER,
	CLIENT
};

// utils/debug.cpp
void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg = "");
void	display_epoll_event(const struct epoll_event& event, const std::string& msg = "");

template <typename T>
std::string	nbtostr(T nb)
{
	std::ostringstream ss;
	ss << nb;
	return ss.str();
}


#endif