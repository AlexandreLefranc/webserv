#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstdio>
# include <stdlib.h>
# include <fstream>
# include <ios>

# include "webserv.hpp"
# include "config/ServerConfig.hpp"
# include "request/Request.hpp"
# include "cgi/CGI.hpp"

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
	Status					response_status;
	string_map				headers;
	std::vector<char>		body;
	const Request&			request;
	const ServerConfig&		config;
	const ServerLocation*	location_addr;

	Response();
	Response(const Response& other);
	Response&	operator=(const Response& other);

public:
	Response(const Request& request, const ServerConfig& config);
	~Response();

	void	create();
	void	send(int fd) const;

	// Getters
	const Status&				get_status() const;
	const string_map&			get_headers() const;
	const std::vector<char>&	get_body() const;

private:
	void		_serve(std::string& target);
	//	Get Request
	void		_serve_get(std::string& target);
	void		_fetch_ressource(const std::string& target);
	//	Post Request
	void		_serve_post(const std::string& target, const std::vector<char>& content);
	//	Delete Request
	void		_serve_delete(const std::string& target);

	//	Utils
	void	_add_header(const std::string& key, const std::string& value);
	bool	_is_directory(const std::string& location) const;
};

#endif