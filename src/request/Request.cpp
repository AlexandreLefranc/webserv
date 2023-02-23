#include "Request.hpp"

Request::Request()
	: _client_fd(-1)
	, _has_start_line(false)
	, _is_header_done(false)
	, _has_body(false)
	, _body_len(0)
{}

Request::~Request()
{}

/*******************************************************************************
                                PRIVATE METHODS
*******************************************************************************/

std::string		Request::_get_line()
{
	std::string	line = _raw.substr(0, _raw.find("\r\n"));
	_raw.erase(0, _raw.find("\r\n") + 2);

	return line;
}



void	Request::_process_start_line()
{
	std::string line = _get_line();

	std::vector<std::string>	splitted = split(line, " ");
	if (splitted.size() != 3)
	{
		std::cout << YEL << "[Request] 501 Not Implemented" << CRESET << std::endl;
		send(_client_fd, "501 Not Implemented\r\n", 22, 0);
		throw CloseClientException();
	}

	if (splitted[0] != "GET" && splitted[0] != "POST" && splitted[0] != "DELETE")
	{
		std::cout << YEL << "[Request] 501 Not Implemented" << CRESET << std::endl;
		send(_client_fd, "501 Not Implemented\r\n", 22, 0);
		throw CloseClientException();
	}

	if (splitted[2] != "HTTP/1.1")
	{
		std::cout << YEL << "[Request] 505 HTTP Version Not Supported" << CRESET << std::endl;
		send(_client_fd, "505 HTTP Version Not Supported\r\n", 33, 0);
		throw CloseClientException();
	}

	_method = splitted[0];
	_target = splitted[1];
	_protocol = splitted[2];
	_has_start_line = true;
	std::cout << YEL << "[Request] Start Line OK" << CRESET << std::endl;
	std::cout << _method + " " + _target + " " + _protocol << std::endl;
}

// Returns true if request is complete. false otherwise
bool	Request::_process_header()
{
	std::string	line = _get_line();

	if (line == "")
	{
		_check_headers();
		_is_header_done = true;
		std::cout << YEL << "[Request] Headers OK" << CRESET << std::endl;
		display_map(_headers, "header");

		if (_has_body == false)
		{
			// std::cout << YEL << "[Request] Ready to respond" << CRESET << std::endl;
			return true;
		}
		else
		{
			std::cout << YEL << "[Request] Listen for body" << CRESET << std::endl;
			return false;
		}
	}
	
	if (std::count(line.begin(), line.end(), ':') == 0)
	{
		std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
		send(_client_fd, "400 Bad Request\r\n", 18, 0);
		throw CloseClientException();
	}

	std::vector<std::string>	splitted = split_first(line, ":");
	std::string					key = tolowerstr(trim(splitted[0]));
	std::string					value = trim(splitted[1]);
	_headers[key] = value;

	return false;
}

void	Request::_check_headers()
{
	if (_headers.find("host") == _headers.end())
	{
		std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
		send(_client_fd, "400 Bad Request\r\n", 18, 0);
		throw CloseClientException();
	}

	if (_headers.find("content-length") != _headers.end())
	{
		_has_body = true;
		_body_len = std::atol(_headers["content-length"].c_str());
		_body_type = "content-length";
	}

	if (_headers.find("transfer-encoding") != _headers.end())
	{
		_has_body = true;
		_body_type = "transfer-encoding";
	}

	return;
}

// Returns true if request is complete. false otherwise
bool	Request::_process_body()
{
	if (_body_type == "content-length" && _body_len > 0)
	{
		if (_body_len > _raw.length())
		{
			_body += _raw;
			_body_len -= _raw.length();
			_raw.clear();
			return false;
		}

		if (_body_len <= _raw.length())
		{
			std::string	sub = _raw.substr(0, _body_len);
			_body += sub;
			_body_len -= sub.length();
			_raw.erase(0, sub.length());
			return true;
		}
	}

	return true;
}

/*******************************************************************************
                                PUBLIC METHODS
*******************************************************************************/

void	Request::set_client_fd(int client_fd)
{
	_client_fd = client_fd;
}

// Returns true if request is complete. false otherwise
bool	Request::parse_data(const std::string& str)
{
	_raw += str;
	std::cout << "raw:" << std::endl << _raw;

	if (str == "STOP" || str == "STOP\r\n")
	{
		throw StopException(); // /!\ TO REMOVE BEFORE SET AS FINISHED
	}

	while (_has_start_line == false && _raw.find_first_of("\r\n") == 0)
	{
		_raw.erase(0, 2);
	}

	if (_has_start_line == false && _raw.find("\r\n") != std::string::npos)
	{
		_process_start_line();
	}

	while (_is_header_done == false && _raw.find("\r\n") != std::string::npos)
	{
		if (_process_header() == true)
		{
			return true;
		}
	}

	if (_is_header_done == true && _has_body == true)
	{
		if (_process_body() == true)
		{
			std::cout << "body: " << _body << std::endl;
			return true;
		}
	}

	return false;
}