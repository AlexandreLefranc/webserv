#include "Request.hpp"

Request::Request()
	: _client_fd(-1)
	, _has_start_line(false)
	, _is_header_done(false)
	, _ready_to_respond(false)
{}

Request::~Request()
{}

void	Request::set_client_fd(int client_fd)
{
	_client_fd = client_fd;
}

void	Request::parse_data(const std::string& str)
{
	_raw += str;
	std::cout << "raw:" << std::endl << _raw;

	while (_raw.find("\r\n") != std::string::npos)
	{
		std::string	line = _raw.substr(0, _raw.find("\r\n"));
		_raw.erase(0, _raw.find("\r\n") + 2);
		std::cout << "line:" << std::endl << line << std::endl;
		if (_has_start_line == false && line == "")
		{
			continue;
		}

		if (line == "STOP")
		{
			throw StopException(); // /!\ TO REMOVE BEFORE SET AS FINISHED
		}

		if (_has_start_line == false)
		{
			std::vector<std::string>	splitted = split(line, " ");
			if (splitted.size() != 3)
			{
				send(_client_fd, "501 Not Implemented\r\n", 22, 0);
				// throw CloseClientException();
				continue;
			}
			if (splitted[0] != "GET" && splitted[0] != "POST" && splitted[0] != "DELETE")
			{
				send(_client_fd, "501 Not Implemented\r\n", 22, 0);
				// throw CloseClientException();
				continue;
			}
			if (splitted[2] != "HTTP/1.1")
			{
				send(_client_fd, "505 HTTP Version Not Supported\r\n", 33, 0);
				// throw CloseClientException();
				continue;
			}
			_method = splitted[0];
			_target = splitted[1];
			_protocol = splitted[2];
			_has_start_line = true;
			std::cout << "Start Line OK" << std::endl;
		}
		else if (_is_header_done == false)
		{
			if (line == "")
			{
				_is_header_done = true;
				std::cout << "Headers OK" << std::endl;
				continue;
			}
			
			if (std::count(line.begin(), line.end(), ':') == 0)
			{
				send(_client_fd, "400 Bad Request\r\n", 18, 0);
				// throw CloseClientException();
				continue;
			}

			std::vector<std::string>	splitted = split_first(line, ":");
			_headers[tolowerstr(trim(splitted[0]))] = trim(splitted[1]);
			display_map(_headers, "header");
		}
	}
}