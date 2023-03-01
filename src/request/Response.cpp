# include "Response.hpp"

/*==============================================================================

									STATUS.

==============================================================================*/

Status::Status(int code, std::string msg)
	: protocol("HTTP/1.1")
	, code(code)
	, message(msg)
{
	return ;
}

const Status Status::Forbidden = Status(403, "Not Allowed");
const Status Status::NotFound = Status(404, "Not Found");
const Status Status::OK = Status(200, "OK");
const Status Status::Created = Status(201, "Created");
const Status Status::NoContent = Status(204, "No Content");

/*==============================================================================

									Response.

==============================================================================*/

/*==============================================================================
								Constructors.
==============================================================================*/

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
	std::string	target;
	
	_config = config;
	_add_header("Server", "Webserv42/1.0");
	_add_header("Connection", "close");
	try
	{
		target = _config.get_target(request.get_target(), request.get_method());
	}
	catch (ParsingException& e)
	{
		_status = Status::Forbidden;
		return ;
	}
	if (request.get_method() == GET)
		_serve_get(target);
	else if (request.get_method() == POST)
		_serve_post(target);
	else if (request.get_method() == DELETE)
		_serve_delete(target);
	return ;
}

void	Response::send(int fd) const
{
	return ;
}

/*==============================================================================

							Private member functions.

==============================================================================*/

/*==============================================================================
								Get Response.
==============================================================================*/

void	Response::_serve_get(const std::string& target)
{
	_fetch_ressource(target);
	if (_body.size() > 0)
	{
		_add_header("Content-Length", std::to_string(_body.length()));
		// _add_header("Content-Type", _get_content_type(request.get_target()));
	}
	return ;
}

void	Response::_fetch_ressource(const std::string& target)
{
	std::ifstream		file(target);
	std::stringstream	buffer;
	
	if (!file.is_open())
	{
		_status = Status::NotFound;
		return ;
	}
	buffer << file.rdbuf();
	_body = buffer.str();
	_status = Status::OK;
	return ;
}

/*==============================================================================
								Post Response.
==============================================================================*/

void	Response::_serve_post(const std::string& target, const char* content, size_t content_length)
{
	std::ofstream	file(target, POST_mode);

	if (!file.is_open(target))
	{
		_status = Status::Forbidden;
		return ;
	}
	file.write(content, content_length);
	file.close();
	_status = OK;
	return ;
}

/*==============================================================================
								Delete Response.
==============================================================================*/

void	Response::_serve_delete(const std::string& target)
{
	if (std::remove(target) == 0)
		_status = OK;
	else
		_status = NoContent;
	return ;
}

/*==============================================================================
								Utils.
==============================================================================*/

void	Response::_add_header(std::string key, std::string value)
{
	_headers[key] = value;
	return ;
}