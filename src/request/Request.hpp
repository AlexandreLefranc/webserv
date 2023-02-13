#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

struct Request
{
private: // Disable defaults behaviors
	Request(const Request& src);
	Request&	operator=(const Request& src);

public:
	std::string							raw;

	std::string							method;
	std::string							route;
	std::string							protocol;

	std::map<std::string, std::string>	headers;

	Request();
	~Request();
};

#endif