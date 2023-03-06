#include "ResponseGenerator.hpp"

void	ResponseGenerator::send_error(int code, const std::string& desc, int client_fd)
{
	std::string	body = HTMLGenerator::error(code, desc);

	std::stringstream	ss;
	ss << "HTTP/1.1 " << code << " " << desc << "\r\n";
	ss << "Content-Type: text/html\r\n";
	ss << "Content-Length: " << body.length() << "\r\n";
	ss << "Connection: close\r\n";
	ss << "Server: webserv/1.0\r\n";
	ss << "\r\n";
	ss << body;

	send(client_fd, ss.str().c_str(), ss.str().length(), MSG_DONTWAIT);
}