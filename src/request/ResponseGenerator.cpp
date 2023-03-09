#include "ResponseGenerator.hpp"

void	ResponseGenerator::send_error(int code, const std::string& desc, int client_fd)
{
	std::vector<char>	body = HTMLGenerator::error(code, desc);

	std::stringstream	ss;
	std::string			buffer;
	std::vector<char>	message;

	ss << "HTTP/1.1 " << code << " " << desc << "\r\n";
	ss << "Content-Type: text/html\r\n";
	ss << "Content-Length: " << body.size() << "\r\n";
	ss << "Connection: close\r\n";
	ss << "Server: webserv/1.0\r\n";
	ss << "\r\n";

	buffer = ss.str();
	message.reserve(buffer.size() + body.size());
	message.assign(buffer.begin(), buffer.end());
	message.insert(message.end(), body.begin(), body.end());

	send(client_fd, message.data(), message.size(), MSG_DONTWAIT);
}