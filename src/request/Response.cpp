# include "Response.hpp"

Response::Response()
{
	return ;
}

Response::~Response()
{
	return ;
}

Response::Response(const Response& other)
{
	if (this != &other)
		*this = other;
	return ;
}

Response&	Response::operator=(const Response& other)
{
	return (*this);
}

void	Response::create(const Request& request, const ServerConfig& config)
{
	_fetch_ressource(config.get_root() + request.get_target());
	// _make_status();
	_add_header("Server", "Webserv42/1.0");
	_add_header("Connection", "close");
	if (_body.size() > 0)
	{
		_add_header("Content-Length", std::to_string(_body.length()));
		_add_header("Content-Type", _get_content_type(request.get_target()));
	}
	return ;
}

void	_fetch_ressource(std::string target)
{
	std::ifstream		file(target);
	std::stringstream	buffer;
	
	if (!file.is_open())
	{
		_status.code = 404;
		return ;
	}
	buffer << file.rdbuf();
	_body = buffer.str();
}

// void	Response::_make_status()
// {
// 	_status.code = 404;
// 	_status.message = "Page not found.";
// }

void	Response::_add_header(std::string key, std::string value)
{
	_headers[key] = value;
	return ;
}

Response::