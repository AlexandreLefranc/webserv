
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstdio>

# include "webserv.hpp"
# include "config/ServerConfig.hpp"
# include "request/Request.hpp"

# define POST_mode	std::ios_base::out

struct Status
{
	std::string	protocol;
	int			code;
	std::string	message;

	Status();
	Status(int code, std::string msg);

	static const Status	Forbidden;
	static const Status	NotFound;
	static const Status	OK;
	static const Status	Created;
	static const Status	NoContent;
};

class Response
{
private:
	Status								_status;
	std::map<std::string, std::string>	_headers;
	std::vector<char>					_body;
	ServerConfig						_config;

public:
	Response();
	~Response();
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
	void	_serve_delete(const std::string& target);

	//	Utils
	void	_add_header(std::string key, std::string value);
};

#endif