
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstdio>

# include "webserv.hpp"

# define POST_mode	std::ios_base::out

struct Status
{
	std::string	protocol("HTTP/1.1");
	int			code;
	std::string	message;

	static const Status	Forbidden;
	static const Status	NotFound;
	static const Status	OK;
	static const Status	Created;
	static const Status	NoContent;
};

class Response
{
private:
	t_status										_status;
	std::map<std::string key, std::string value>	_headers;
	std::vector<char>								_body;
	const ServerConfig&								_config;

public:
	Response();
	// ~Response();
	Response(const Response& other);
	Response&	operator=(const Response& other);

	void	create(const Request& request, const ServerConfig& config);
	void	send(int fd) const;

private:
	//	Get Request
	void	_serve_get(const std::string& target);
	void	_fetch_ressource(const std::string& target);
	//	Post Request
	void	_serve_post(const std::string& target, const std::string& content, size_t content_length);
	//	Delete Request
	void	_serve_get(const std::string& target);

	//	Utils
	void	_add_header(std::string key, std::string value);
}

#endif