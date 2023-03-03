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
	std::string	line = _raw_s.substr(0, _raw_s.find("\r\n"));
	_raw_d.erase(_raw_d.begin(), _raw_d.begin() + line.length() + 2);
	_raw_s.erase(_raw_s.begin(), _raw_s.begin() + line.length() + 2);

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
	_process_target(splitted[1]);
	_protocol = splitted[2];
	_has_start_line = true;

	std::cout << YEL << "[Request] Start Line OK" << CRESET << std::endl;
	std::cout << _method + " " + _target + " " + _protocol << std::endl;
	display_map(_target_param, "target_param");
}

void	Request::_process_target(const std::string& target)
{
	std::vector<std::string>	target_split = split_first(target, "?");

	_target = target_split[0];

	if (target_split.size() == 1)
	{
		return;
	}

	std::vector<std::string>	param_split = split(target_split[1], "&");

	std::vector<std::string>::const_iterator it;
	for (it = param_split.begin(); it != param_split.end(); ++it)
	{
		std::vector<std::string>	equal_split = split_first(*it, "=");
		
		if (equal_split.size() == 1)
		{
			std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
			send(_client_fd, "400 Bad Request\r\n", 18, 0);
			throw CloseClientException();
		}

		_target_param[equal_split[0]] = equal_split[1];
	}
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
		if (_body_len > _raw_d.size())
		{
			_body.insert(_body.end(), _raw_d.begin(), _raw_d.end());
			_body_len -= _raw_d.size();
			_raw_d.clear();
			_raw_s.clear();
			return false;
		}

		if (_body_len <= _raw_d.size())
		{
			_body.insert(_body.end(), _raw_d.begin(), _raw_d.begin() + _body_len);

			_body_len = 0;
			_raw_d.clear();
			_raw_s.clear();
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
bool	Request::parse_data(const std::vector<char>& data)
{
	std::string data_s;
	data_s.insert(data_s.end(), data.begin(), data.end());

	_raw_d.insert(_raw_d.end(), data.begin(), data.end());
	_raw_s.insert(_raw_s.end(), data.begin(), data.end());

	std::cout << "raw_d size:" << _raw_d.size() << " | _raw_s len:" <<  _raw_s.length() << std::endl;
	std::cout << "raw:" << std::endl << _raw_s;

	if (data_s == "STOP" || data_s == "STOP\r\n")
	{
		throw StopException(); // /!\ TO REMOVE BEFORE SET AS FINISHED
	}

	while (_has_start_line == false && _raw_s.find_first_of("\r\n") == 0)
	{
		std::cout << "Empty line before start of request" << std::endl;
		_raw_d.erase(_raw_d.begin(), _raw_d.begin() + 2);
		_raw_s.erase(_raw_s.begin(), _raw_s.begin() + 2);
	}

	if (_has_start_line == false && _raw_s.find("\r\n") != std::string::npos)
	{
		_process_start_line();
	}

	while (_is_header_done == false && _raw_s.find("\r\n") != std::string::npos)
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
			// debug
			std::string body;
			body.insert(body.end(), _body.begin(), _body.end());
			std::cout << "body: " << body << std::endl;

			return true;
		}
	}

	return false;
}

/*==============================================================================
	Getters.
==============================================================================*/

const std::string&	Request::get_target() const
{
	return (_target);
}

const std::string&	Request::get_method() const
{
	return (_method);
}

std::string			Request::get_header(std::string key) const
{
	try
	{
		return (_headers.at(key));
	}
	catch (std::out_of_range& e)
	{
		throw (ResponseException());
	}
}

const std::vector<char>&	Request::get_body() const
{
	return (_body);
}