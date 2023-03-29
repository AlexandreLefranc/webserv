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

bool	Status::is_error() const
{
	if (this->code >= 300)
		return (true);
	else
		return (false);
}

const Status Status::OK = Status(200, "OK");
const Status Status::Created = Status(201, "Created");
const Status Status::NoContent = Status(204, "No Content");
const Status Status::MovedPermanently = Status(301, "Moved Permanently");
const Status Status::BadRequest = Status(400, "Bad Request");
const Status Status::Forbidden = Status(403, "Not Allowed");
const Status Status::NotFound = Status(404, "Not Found");
const Status Status::MethodNotAllowed = Status(405, "Method Not Allowed");
const Status Status::ContentTooLarge = Status(413, "Content Too Large");
const Status Status::InternalServerError = Status(500, "Internal Server Error");
const Status Status::NotImplemented = Status(501, "Not Implemented");
const Status Status::HTTPVersionNotSupported = Status(505, "HTTP Version Not Supported");

/*==============================================================================

									Response.

==============================================================================*/

/*==============================================================================
								Constructors.
==============================================================================*/

Response::Response(const Request& request)
	: ready(false)
	, request(request)
	, config(NULL)
	, location_addr(NULL)
{
	return ;
}

Response::~Response()
{
	return ;
}

// /*==============================================================================
// 							Public Member functions.
// ==============================================================================*/

void	Response::create()
{
	std::string	target = request.get_target();
	
	if (config == NULL)
		throw std::runtime_error("Config not set before creating response");

	location_addr = config->get_location_addr(target); // throws ResponseException();
	_add_header("Server", "Webserv42/1.0");
	_add_header("Connection", "close");
	std::cout << YEL << "[RESPONSE]location_match: " << location_addr->get_location_match() << CRESET << std::endl;
	std::cout << YEL << "[RESPONSE]location root: " << location_addr->get_root() << CRESET << std::endl;
	if (location_addr->get_methods().count(request.get_method()) == 0)
	{
		std::cout << YEL << "[Response]Forbidden" << CRESET << std::endl;
		response_status = Status::MethodNotAllowed;
		ready = true;
		return ;
	}
	if (!location_addr->get_redirect().empty())
	{
		std::cout << YEL << "[Response]MovedPermanently" << CRESET << std::endl;
		response_status = Status::MovedPermanently;
		_add_header("Location", location_addr->get_redirect());
		ready = true;
		return ;
	}
	target = location_addr->get_root() + target;
	std::cout << YEL << "[Response] Target: " << target << CRESET << std::endl;
	_serve(target);
	if (response_status.is_error() && config->get_error_page().count(response_status.code) == 1)
		_get_predefined_error_page();
	else if (response_status.is_error())
		_get_default_error_page();
	ready = true;
}

void	Response::create_error(int status_code)
{
	if (status_code == 400)
		response_status = Status::BadRequest;
	else if (status_code == 413)
		response_status = Status::ContentTooLarge;
	else if (status_code == 500)
		response_status = Status::InternalServerError;
	else if (status_code == 501)
		response_status = Status::NotImplemented;
	else if (status_code == 505)
		response_status = Status::HTTPVersionNotSupported;
	else
		throw (ResponseException());

	if (config != NULL && config->get_error_page().count(response_status.code) == 1)
		_get_predefined_error_page();
	else
		_get_default_error_page();
	ready = true;
}

/*==============================================================================
								Setters.
==============================================================================*/

void						Response::set_config(const ServerConfig* conf)
{
	if (config != NULL)
		throw std::runtime_error("Config already set");
	std::cout << "Setting config in Response" << std::endl;
	config = conf;
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

/*==============================================================================
								Get Response.
==============================================================================*/

void	Response::_serve_get(std::string& target)
{
	std::cout << "Serving GET - " << target << std::endl;
	if (is_directory(target) && location_addr->get_autoindex() == true)
	{
		std::cout << "Directory with autoindex" << std::endl;
		body = HTMLGenerator::dirlist(target, request.get_target());
		// std::cout << "Body :" << std::string(body.begin(), body.end()) << std::endl;
		_add_header("Content-Length", itos(body.size()));
		response_status = Status::OK;
		return ;
	}
	if (is_directory(target))
	{
		std::cout << "Directory without autoindex" << std::endl;
		target = target + location_addr->get_index();
	}
	
	if (_is_cgi_file(target))
	{
		std::cout << YEL << "[Response] GET with CGI." << CRESET << std::endl;
		_call_cgi();
	}
	else
	{
		std::cout << YEL << "[Response] GET on regular file." << CRESET << std::endl;
		_fetch_ressource(target);
	}

	if (body.size() > 0)
	{
		_add_header("Content-Length", itos(body.size()));
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
	if (is_directory(target) && request._body_type == "form-data")
	{
		std::cout << YEL << "[Response] POST form to dir." << CRESET << std::endl;
		// Allow file uploading.
		_upload_file(target);
	}
	else if (_is_cgi_file(target) &&\
		(request._body_type == "urlencoded" || request._body_type == "form-data"))
	{
		std::cout << YEL << "[Response] POST with CGI." << CRESET << std::endl;
		_call_cgi();
	}
	else
	{
		std::cout << YEL << "[Response] POST on not allowed target" << CRESET << std::endl;
		response_status = Status::Forbidden;
	}
}

void	Response::_upload_file(const std::string& target)
{
	std::string			filename = target + request.get_body().front().get_header("filename");
	std::ofstream		ofs(filename.c_str());
	std::vector<char>	content;

	std::cout << YEL << "Uploading File: " << filename << CRESET << std::endl;
	if (!ofs.is_open())
	{
		std::cout << YEL << "Could not open file." << CRESET << std::endl;
		response_status = Status::Forbidden;
		return ;
	}
	std::cout << YEL << "[RESPONSE]File opened: " << filename << CRESET << std::endl;
	content = request.get_body().front().get_body();
	ofs.write(content.data(), content.size());
	ofs.close();
	if (ofs.fail())
	{
		std::remove((filename).c_str());
		throw (ResponseException());
	}
	response_status = Status::Created;
	return ;
}

/*==============================================================================
								Delete Response.
==============================================================================*/

void	Response::_serve_delete(const std::string& target)
{
	//	Secure
	if (is_directory(target))
		response_status = Status::Forbidden;
	else if (std::remove(target.c_str()) == 0)
		response_status = Status::OK;
	else
		response_status = Status::NoContent;
	return ;
}

/*==============================================================================
								Build Response.
==============================================================================*/

std::vector<char>	Response::build_response_vector() const
{
	std::stringstream	ss_response;
	std::string			buffer;
	std::vector<char>	page;

	ss_response	<< response_status.protocol << " " << response_status.code
				<< " " << response_status.message << "\r\n";
	for (string_map::const_iterator it = headers.begin(); it != headers.end(); ++it)
		ss_response << it->first << ": " << it->second << "\r\n";
	ss_response << "\r\n";
	buffer = ss_response.str();
	page.assign(buffer.begin(), buffer.end());
	page.insert(page.end(), body.begin(), body.end());
	return (page);
}

void	Response::_get_predefined_error_page()
{
	std::ifstream		file(config->get_error_page().at(response_status.code).c_str());
	std::stringstream	buffer;
	
	if (!file.is_open())
	{
		_get_default_error_page();
		return ;
	}	
	try
	{
		body = read_file(file);
	}
	catch (std::ios::failure& e)
	{
		_get_default_error_page();
		return ;
	}
	if (body.size() > 0)
		_add_header("Content-Length", itos(body.size()));
	else
		_get_default_error_page();
}

void	Response::_get_default_error_page()
{
	body = HTMLGenerator::error(response_status.code, response_status.message);
	_add_header("Content-Length", itos(body.size()));
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

bool	Response::_is_cgi_file(const std::string& target) const
{
	if (target.find('.') != std::string::npos &&\
			target.substr(target.find_last_of('.')) == config->get_cgi().first)
		return true;
	return false;
}

void	Response::_call_cgi()
{
	CGI cgi(config->get_cgi().second, location_addr->get_root(), *config, request);
	cgi.process();

	if (cgi.cgi_headers.find("status") != cgi.cgi_headers.end())
	{
		int			cgi_code = std::atoi(cgi.cgi_headers["status"].c_str());
		std::string	cgi_msg = cgi.cgi_headers["status"].substr(5);
		response_status = Status(cgi_code, cgi_msg);
	}
	else
	{
		response_status = Status::OK;
	}

	headers.insert(cgi.cgi_headers.begin(), cgi.cgi_headers.end());
	body = cgi.cgi_body;
}