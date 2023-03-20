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

Response::Response(const Request& request, const ServerConfig& config)
	: request(request)
	, config(config)
{
	return ;
}

Response::~Response()
{
	return ;
}

/*==============================================================================
							Public Member functions.
==============================================================================*/

void	Response::create()
{
	std::string	target = request.get_target();
	
	location_addr = config.get_location_addr(target); // throws ResponseException();
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
	std::cout << YEL << "[Response] Target: " << target << CRESET << std::endl;
	_serve(target);
}

void	Response::send(int fd) const
{
	(void)fd;
	return ;
}

/*==============================================================================
								Getters.
==============================================================================*/

const Status&				Response::get_status() const
{
	return response_status;
}

const string_map&			Response::get_headers() const
{
	return headers;
}

const std::vector<char>&	Response::get_body() const
{
	return body;
}


/*==============================================================================

							Private member functions.

==============================================================================*/

/*==============================================================================
								Get Response.
==============================================================================*/

void	Response::_serve(std::string& target)
{
	if (request.get_method() == "GET")
		_serve_get(target);
	else if (request.get_method() == "POST")
		_serve_post(target);
	else if (request.get_method() == "DELETE")
		_serve_delete(target);
	return ;
}

void	Response::_serve_get(std::string& target)
{
	std::cout << "Serving GET" << std::endl;
	if (_is_directory(target) && location_addr->get_autoindex() == true)
	{
		std::cout << "Dir + autoindex" << std::endl;
		body = HTMLGenerator::dirlist(target, request.get_target());
		std::cout << std::string(body.begin(), body.end()) << std::endl;
		_add_header("Content-Type", "text/html");
		_add_header("Content-Length", itos(body.size()));
		response_status = Status::OK;
		return ;
	}
	if (_is_directory(target))
	{
		std::cout << "Dir - autoindex" << std::endl;
		target = target + location_addr->get_index();
	}

	if (target.substr(target.find_last_of('.')) == config.get_cgi().first)
	{
		std::cout << YEL << "[Response]Called CGI." << CRESET << std::endl;
		CGI cgi(config.get_cgi().second, location_addr->get_root(), config, request);
		cgi.process();
		if (cgi.cgi_headers.find("status") != cgi.cgi_headers.end())
		{
			int cgi_code = std::atoi(cgi.cgi_headers["status"].c_str());
			response_status = Status(cgi_code, "");
		}
		else
		{
			response_status = Status::OK;
		}
		headers.insert(cgi.cgi_headers.begin(), cgi.cgi_headers.end());
		body = cgi.cgi_body;
	}
	else
	{
		std::cout << "File no cgi" << std::endl;
		_fetch_ressource(target);
	}
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
	try
	{
		body = read_file(file);
	}
	catch (std::ios::failure& e)
	{
		response_status = Status::Forbidden;
		return ;
	}
	response_status = Status::OK;
	return ;
}

/*==============================================================================
								Post Response.
==============================================================================*/

void	Response::_serve_post(const std::string& target)
{
	if (request._body_type == "urlencoded")
	{
		std::cout << YEL << "[Response]POST url to CGI." << CRESET << std::endl;
		// Do CGI stuff with arguments in body.
	}
	else if (request._body_type == "form-data")
	{
		if (_is_directory(target))
		{
			std::cout << YEL << "[Response]POST form to dir." << CRESET << std::endl;
			// Allow file uploading.
			_upload_file(target);
		}
		else
		{
			std::cout << YEL << "[Response]POST form to CGI." << CRESET << std::endl;
			// Do CGI stuff with arguments in body but multi-part.
		}
	}
	else if (request._body_type == "plain")
	{
		std::cout << YEL << "[Response]POST plain text ??" << CRESET << std::endl;
		// Fuck off.
	}
	return ;
}

void	Response::_upload_file(const std::string& target)
{
	std::string							filename = _get_filename();
	std::ofstream						ofs((target + filename).c_str());
	std::vector<char>::const_iterator	it;

	std::cout << YEL << "Uploading File: " << filename << CRESET << std::endl;
	if (!ofs.is_open())
	{
		response_status = Status::Forbidden;
		return ;
	}
	it = vec_find(request._body, "\n\n");
	ofs.write(it.base(), vec_find(request._body, request._headers.at("boundary") + "--") - it);
	ofs.close();
	if (ofs.fail())
	{
		std::remove((target + filename).c_str());
		throw (ResponseException());
	}
	response_status = Status::Created;
	return ;
}

std::string	Response::_get_filename() const
{
	std::string	boundary = request._headers.at("boundary");
	std::string	str_body = std::string(request._body.begin(), request._body.end());
	size_t		pos_filename;

	std::cout << YEL << "[Request]body: " << str_body.substr(15) << "..." << CRESET << std::endl;
	std::cout << YEL << "[POST Response]boundary: " << boundary << CRESET << std::endl;
	if (str_body.find(boundary) != 0)
		throw (ResponseException());
	str_body = str_body.substr(boundary.length() + 1);
	std::cout << YEL << "[Request]removed boundary: " << str_body.substr(15) << "..." << CRESET << std::endl;
	pos_filename = str_body.find("filename=");
	if (pos_filename < 0)
		throw (ResponseException());
	return (str_body.substr(str_body.find('\"', pos_filename), str_body.find('\"', str_body.find('\"', pos_filename) + 1)));
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

void	Response::_add_header(const std::string& key, const std::string& value)
{
	if (headers.find(key) == headers.end())
		headers[key] = value;
	return ;
}

bool	Response::_is_directory(const std::string& location) const
{
	if (location.size() && location[location.size() - 1] == '/')
		return (true);
	else
		return (false);
}