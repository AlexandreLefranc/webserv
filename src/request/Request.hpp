#ifndef REQUEST_HPP
# define REQUEST_HPP

struct Request
{
private: // Disable defaults behaviors
	Request();
	Request(const Request& src);
	Request&	operator=(const Request& src);

public:
	std::string	raw;

	void	receive_all(int fd);

};

#endif