# include "Response.hpp"

/*==============================================================================

									STATUS.

==============================================================================*/

Status::Status()
{
	return ;
}

Status::Status(int code, std::string msg)
	: protocol("HTTP/1.1")
	, code(code)
	, message(msg)
{
	return ;
}

const Status Status::OK = Status(200, "OK");
const Status Status::Created = Status(201, "Created");
const Status Status::NoContent = Status(204, "No Content");
const Status Status::MovedPermanently = Status(301, "Moved Permanently");
const Status Status::Forbidden = Status(403, "Not Allowed");
const Status Status::NotFound = Status(404, "Not Found");

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

// Response::Response(const Response& other)
// {
// 	if (this != &other)
// 		*this = other;
// 	return ;
// }

// Response&	Response::operator=(const Response& other)
// {
// 	if (this != &other)
// 	{
// 		response_status = other.response_status;
// 		headers = other.headers;
// 		body = other.body;
// 		config = other.config;
// 		location = other.location;
// 	}
// 	return (*this);
// }

void	Response::create(const Request& request, const ServerConfig& server_config)
{
	std::string	target = request.get_target();
	
	config_addr = &server_config;
	location_addr = config_addr->get_location_addr(target); // throws ResponseError();
	_add_header("Server", "Webserv42/1.0");
	_add_header("Connection", "close");
	if (location_addr->get_methods().count(request.get_method()) == 0)
	{
		std::cout << YEL << "[Response]Forbidden" << CRESET << std::endl;
		response_status = Status::Forbidden;
		return ;
	}
	if (!location_addr->get_redirect().empty())
	{
		std::cout << YEL << "[Response]MovedPermanently" << CRESET << std::endl;
		response_status = Status::MovedPermanently;
		_add_header("Location", location_addr->get_redirect());
		return ;
	}
	target = location_addr->get_root() + target;
	std::cout << YEL << "Target: " << target << CRESET << std::endl;
	// _serve(request, target);
}

void	Response::send(int fd) const
{
	(void)fd;
	return ;
}

/*==============================================================================

							Private member functions.

==============================================================================*/

/*==============================================================================
								Get Response.
==============================================================================*/

void	Response::_serve(const Request& request, std::string target)
{
	if (request.get_method() == "GET")
		_serve_get(target);
	else if (request.get_method() == "POST")
		_serve_post(target, request.get_body());
	else if (request.get_method() == "DELETE")
		_serve_delete(target);
	return ;
}

void	Response::_serve_get(const std::string& target)
{
	// if (_is_directory(target))
	// {
	// 	// make directory listing body.
	// 	return ;
	// }
	_fetch_ressource(target + location_addr->get_index());
	if (body.size() > 0)
	{
		_add_header("Content-Length", itos(body.size()));
		// _add_header("Content-Type", _get_content_type(request.get_target()));
	}
	return ;
}

void	Response::_fetch_ressource(const std::string& target)
{
	std::ifstream		file(target.c_str());
	std::stringstream	buffer;
	std::string			buffer_str;
	
	if (!file.is_open())
	{
		response_status = Status::NotFound;
		return ;
	}
	buffer << file.rdbuf();
	// buffer_str = buffer.str();
	body.insert(body.begin(), buffer_str.begin(), buffer_str.end());
	response_status = Status::OK;
	return ;
}

/*==============================================================================
								Post Response.
==============================================================================*/

void	Response::_serve_post(const std::string& target, const std::vector<char>& content)
{
	std::ofstream	file(target.c_str());
	
	if (!file.is_open())
	{
		response_status = Status::Forbidden;
		return ;
	}
	for (std::vector<char>::const_iterator it = content.begin(); it != content.end(); it++)
		file.put(*it);
	// file.write(content, content.size());
	file.close();
	response_status = Status::OK;
	return ;
}



/*==============================================================================
								Delete Response.
==============================================================================*/

void	Response::_serve_delete(const std::string& target)
{
	if (std::remove(target.c_str()) == 0)
		response_status = Status::OK;
	else
		response_status = Status::NoContent;
	return ;
}

/*==============================================================================
								Utils.
==============================================================================*/

void	Response::_add_header(std::string key, std::string value)
{
	headers[key] = value;
	return ;
}

// bool	Response::_is_directory(std::string location) const
// {
// 	if (location.back() == "/")
// 		return (true);
// 	else
// 		return (false);
// }