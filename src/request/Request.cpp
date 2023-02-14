#include "Request.hpp"

Request::Request()
	: _client_fd(-1), _has_start_line(false)
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
			_route = splitted[1];
			_protocol = splitted[2];
			_has_start_line = true;
		}
		else
		{
			
		}
	}
}



// int		HTTPServer::_process_epollin(const epoll_event& event)
// {
// 	Client&		client = _client_manager.get_client(event.data.fd);

// 	std::string str = _receive_all(event.data.fd);
// 	client.request.raw += str;
// 	// std::cout << "raw:" << std::endl << client.request.raw;

// 	while (client.request.raw.find("\r\n") != std::string::npos)
// 	{
// 		std::string line = client.request.raw.substr(0, client.request.raw.find("\r\n"));
// 		client.request.raw.erase(0, client.request.raw.find("\r\n") + 2);
// 		std::cout << "line:" << std::endl << line << std::endl;

// 		std::vector<std::string>	splitted = split(line, " ");
// 		if (client.request.method == "" && splitted.size() != 3)
// 		{
// 			std::cout << "Send 501 Not Implemented" << std::endl;
// 		}
// 		else
// 		{
// 			client.request.method = splitted[0];
// 			client.request.route = splitted[1];
// 			client.request.protocol = splitted[2];
// 			// check validy
// 		}
// 	}

// 	if ((event.events & EPOLLOUT) != 0)
// 	{
// 		send(event.data.fd, "GOT IT !\n", 10, 0);
// 	}
// 	// _epoll.remove_fd(event.data.fd);
// 	// _client_manager.remove_client(event.data.fd);
// 	// _fds.erase(event.data.fd);
// 	if (str == "STOP\r\n" || str == "STOP")
// 	{
// 		return -1;
// 	}
// 	return 0;
// }