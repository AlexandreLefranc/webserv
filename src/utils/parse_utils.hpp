#ifndef PARSE_UTILS_HPP
# define PARSE_UTILS_HPP

# include <exception>
# include <string>
# include <vector>

class ParsingException: public std::exception
{
public:
	const char*	what() const throw();
};

std::string&				format_line(std::string& line)
std::vector<std::string>	split_tokens(std::string line)


#endif