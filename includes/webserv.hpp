#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <algorithm>
# include <sstream>

# include <string>
# include <vector>
# include <map>
# include <utility>
# include <unistd.h>

# include "ansi_colors.hpp"
# include "utils/Exceptions.hpp"

# define EPOLL_SIZE	200
# define BUFF_SIZE	32

extern bool g_running;

typedef std::map<std::string, std::string>	string_map;
typedef std::pair<std::string, std::string>	string_pair;

// utils/debug.cpp
void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg = "");
void	display_epoll_event(const struct epoll_event& event, const std::string& msg = "");
void	display_cstyle_string_array(char** array, const std::string& name);
void	send_example_page(int client_fd);

// utils/filesystem_utils.cpp
std::vector<std::string>	get_dir_list(std::string path);
std::string					gen_dir_list_html(std::vector<std::string> dirlist, const std::string& path);
std::vector<char>			read_file(std::ifstream& ifs);
bool						file_exists(const std::string& filename);
bool						is_directory(const std::string& path);

// utils/string_utils.cpp
std::vector<std::string>	split(std::string s, std::string delimiter);
std::vector<std::string>	split_first(std::string s, std::string delimiter);
std::string					trim(const std::string& str);
std::string					tolowerstr(const std::string& str);
std::string					toupperstr(const std::string& str);
std::string					itos(int number);

// utils/socket_utils.cpp
std::vector<char>	receive_all(int fd);

//	utils/char_vector_utils.cpp
bool								is_equal(std::vector<char> vec, const std::string& str);
std::vector<char>::const_iterator	vec_find(const std::vector<char>& vec, const std::string& substr);
std::vector<char>::const_iterator	vec_find(const std::vector<char>& vec, const std::string& substr, std::vector<char>::const_iterator& start_pos);


# include "utils/debug.tpp"
# include "utils/string_utils.tpp"


#endif