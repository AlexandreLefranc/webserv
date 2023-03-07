#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <algorithm>
# include <sstream>

# include <string>
# include <vector>
# include <map>

# include "ansi_colors.hpp"
# include "utils/Exceptions.hpp"

# define EPOLL_SIZE	200
# define BUFF_SIZE	32

extern bool g_running;

enum	t_http_method
{
	DELETE,
	GET,
	// HEAD,
	POST
};

// utils/debug.cpp
void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg = "");
void	display_epoll_event(const struct epoll_event& event, const std::string& msg = "");
void	display_cstyle_string_array(char** array, const std::string& name);
void	send_example_page(int client_fd);

// utils/filesystem_utils.cpp
std::vector<std::string> get_dir_list(std::string path);
std::string		gen_dir_list_html(std::vector<std::string> dirlist, const std::string& path);

// utils/string_utils.cpp
std::vector<std::string> split(std::string s, std::string delimiter);
std::vector<std::string>	split_first(std::string s, std::string delimiter);
std::string trim(const std::string& str);
std::string tolowerstr(const std::string& str);
std::string	toupperstr(const std::string& str);

// utils/socket_utils.cpp
std::vector<char>	receive_all(int fd);

# include "utils/debug.tpp"
# include "utils/string_utils.tpp"


#endif