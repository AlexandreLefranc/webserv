#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstdio>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <fstream>
# include <sstream>
# include <ios>
# include <algorithm>

# include "webserv.hpp"
# include "config/ServerConfig.hpp"
# include "request/Request.hpp"
# include "cgi/CGI.hpp"

// # define POST_mode	std::ios_base::out

// const std::map<std::string, std::string>	MIMES = {
// 	{"audio/mp3", "mp3"},
// 	{"text/plain", "conf"}
// };

struct Status
{
	std::string	protocol;
	int			code;
	std::string	message;

	Status();
	Status(int code, std::string msg);

	bool	is_error() const;
	
	static const Status	OK;
	static const Status	Created;
	static const Status	NoContent;
	static const Status	MovedPermanently;
	static const Status	BadRequest;
	static const Status	Forbidden;
	static const Status	NotFound;
	static const Status	MethodNotAllowed;
	static const Status	ContentTooLarge;
	static const Status	InternalServerError;
	static const Status	NotImplemented;
	static const Status	HTTPVersionNotSupported;
};

class Response
{
public:
	bool	ready;
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
	void	create_error(int status_code);
	void	send(int fd) const;

	// Getters
	const Status&				get_status() const;
	const string_map&			get_headers() const;
	const std::vector<char>&	get_body() const;
	//	Build reponse
	std::vector<char>			build_response_vector() const;

private:
	void				_serve(std::string& target);
	//	Get Request
	void				_serve_get(std::string& target);
	void				_fetch_ressource(const std::string& target);
	//	Post Request
	void				_serve_post(const std::string& target);
	void				_upload_file(const std::string& target);
	std::string			_get_filename() const;
	//	Delete Request
	void				_serve_delete(const std::string& target);
	//	Build Response
	void				_get_predefined_error_page();
	void				_get_default_error_page();
	//	Utils
	void				_add_header(const std::string& key, const std::string& value);
	bool				_is_cgi_file(const std::string& target) const;
	void				_call_cgi();
};



#endif