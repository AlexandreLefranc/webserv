#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstdio>
# include <stdlib.h>
# include <fstream>
# include <sys/stat.h>

# include "webserv.hpp"
# include "config/ServerConfig.hpp"
# include "request/Request.hpp"

// # define POST_mode	std::ios_base::out

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
	static const Status	MovedPermanently;
};

class Response
{
private:
	Status								response_status;
	std::map<std::string, std::string>	headers;
	std::vector<char>					body;
	const ServerConfig*					config_addr;
	const ServerLocation*				location_addr;

	Response(const Response& other);
	Response&	operator=(const Response& other);

public:
	Response();
	~Response();

	void	create(const Request& request, const ServerConfig& config);
	void	send(int fd) const;

private:
	void		_serve(const Request& request, std::string target);
	//	Get Request
	void		_serve_get(const std::string& target);
	void		_fetch_ressource(const std::string& target);
	std::string	_itos(int number) const;
	//	Post Request
	void		_serve_post(const std::string& target, const std::vector<char>& content);
	//	Delete Request
	void		_serve_delete(const std::string& target);

	//	Utils
	void	_add_header(std::string key, std::string value);
	bool	_is_directory(std::string location) const;
};

#endif