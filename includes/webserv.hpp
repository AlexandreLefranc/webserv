#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>

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

// utils/string_utils.cpp
std::vector<std::string> split(std::string s, std::string delimiter);
std::vector<std::string>	split_first(std::string s, std::string delimiter);
std::string trim(const std::string& str);
std::string tolowerstr(const std::string& str);

// utils/socket_utils.cpp
std::string	receive_all(int fd);

#endif