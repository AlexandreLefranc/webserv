#ifndef PARSE_UTILS_HPP
# define PARSE_UTILS_HPP

# include <exception>
# include <string>
# include <vector>
# include <sstream>
# include <cstdlib>

# include "webserv.hpp"

std::string&				format_line(std::string& line);
std::vector<std::string>	split_tokens(std::string line);
int							iptoi(const std::string& ip);

#endif