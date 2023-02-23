#include "Request.hpp"

Request::Request()
	: _client_fd(-1)
	, _has_start_line(false)
	, _is_header_done(false)
	, _has_body(false)
	, _body_len(0)
	, _ready_to_respond(false)
{}

Request::~Request()
{}

/*******************************************************************************
                                PRIVATE METHODS
*******************************************************************************/

void	Request::_process_start_line(const std::string& line)
{
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

void	Request::_process_header(const std::string& line)
{
	if (line == "")
	{
		_check_headers();
		_is_header_done = true;
		std::cout << YEL << "[Request] Headers OK" << CRESET << std::endl;
		display_map(_headers, "header");

		if (_has_body == false)
		{
			std::cout << YEL << "[Request] Ready to respond" << CRESET << std::endl;
			_ready_to_respond = true;
		}
		else
		{
			std::cout << YEL << "[Request] Listen for body" << CRESET << std::endl;
		}
		return;
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
	// display_map(_headers, "header");
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

void	Request::_process_body(const std::string& str)
{
	if (_body_type == "content-length" && _body_len > 0)
	{
		if (_body_len >= str.length())
		{
			_body += str;
			_body_len -= str.length();
			std::cout << "_body_len = " << _body_len << std::endl;
		}
		else
		{
			std::string	sub = str.substr(0, _body_len);
			_body += sub;
			_body_len -= sub.length();
			std::cout << "_body_len = " << _body_len << std::endl;
		}
	}
	
	if (_body_type == "content-length" && _body_len == 0)
	{
		std::cout << YEL << "[Request] Ready to respond" << CRESET << std::endl;
		_ready_to_respond = true;
	}
}

/*******************************************************************************
                                PUBLIC METHODS
*******************************************************************************/

void	Request::set_client_fd(int client_fd)
{
	_client_fd = client_fd;
}

bool	Request::parse_data(const std::string& str)
{
	_raw += str;
	std::cout << "raw:" << std::endl << _raw;

	if (str == "STOP" || str == "STOP\r\n")
	{
		throw StopException(); // /!\ TO REMOVE BEFORE SET AS FINISHED
	}

	// Get start line and headers
	while (_raw.find("\r\n") != std::string::npos && _is_header_done == false)
	{
		std::string	line = _raw.substr(0, _raw.find("\r\n"));
		_raw.erase(0, _raw.find("\r\n") + 2);
		// std::cout << "line:" << std::endl << line << std::endl;

		// Request not started yet, we are kind and dont throw
		if (line == "" && _has_start_line == false)
		{
			continue;
		}

		// Start line + headers done
		if (line == "" && _is_header_done == true)
		{
			break;
		}

		if (_has_start_line == false)
		{
			_process_start_line(line);
		}
		else if (_is_header_done == false)
		{
			_process_header(line);
		}
	}

	// Get body if has body
	if (_is_header_done == true && _has_body == true)
	{
		_process_body(_raw);
		_raw.clear();

		std::cout << "Body = " << _body << std::endl;
	}

	if (_ready_to_respond == true)
	{
		std::cout << YEL << "[Request] Responding" << CRESET << std::endl;

		// create_body
		// create_start_line
		// create_headers

		// send_start_line
		// send_headers
		// send_body

		// close connection

		std::string body = "<!doctype html>\n\
<html>\n\
<head>\n\
    <title>Example Domain</title>\n\
    <meta charset=\"utf-8\" />\n\
    <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\" />\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n\
    <style type=\"text/css\">\n\
    body {\n\
        background-color: #f0f0f2;\n\
        margin: 0;\n\
        padding: 0;\n\
        font-family: -apple-system, system-ui, BlinkMacSystemFont, \"Segoe UI\", \"Open Sans\", \"Helvetica Neue\", Helvetica, Arial, sans-serif;\n\
        \n\
    }\n\
    div {\n\
        width: 600px;\n\
        margin: 5em auto;\n\
        padding: 2em;\n\
        background-color: #fdfdff;\n\
        border-radius: 0.5em;\n\
        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);\n\
    }\n\
    a:link, a:visited {\n\
        color: #38488f;\n\
        text-decoration: none;\n\
    }\n\
    @media (max-width: 700px) {\n\
        div {\n\
            margin: 0 auto;\n\
            width: auto;\n\
        }\n\
    }\n\
    </style>    \n\
</head>\n\
\n\
<body>\n\
<div>\n\
    <h1>Example Domain</h1>\n\
    <p>This domain is for use in illustrative examples in documents. You may use this\n\
    domain in literature without prior coordination or asking for permission.</p>\n\
    <p><a href=\"https://www.iana.org/domains/example\">More information...</a></p>\n\
</div>\n\
</body>\n\
</html>";

		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 1256\r\n\r\n" + body;
		send(_client_fd, response.c_str(), response.length(), 0);
		throw CloseClientException();
	}

	return true;
}