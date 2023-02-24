#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "webserv.hpp"

void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg)
{
	std::cout << "[sockaddr_in] " << msg << std::endl;
	std::cout << "  sockaddr_in.sin_family = " << addr.sin_family << std::endl;
	std::cout << "  sockaddr_in.sin_addr.s_addr = " << ntohl(addr.sin_addr.s_addr);
	std::cout << " ("	<< ((ntohl(addr.sin_addr.s_addr) & 0xFF000000) >> 24) << "."
						<< ((ntohl(addr.sin_addr.s_addr) & 0x00FF0000) >> 16) << "."
						<< ((ntohl(addr.sin_addr.s_addr) & 0x0000FF00) >> 8) << "."
						<< ((ntohl(addr.sin_addr.s_addr) & 0x000000FF) >> 0) << ")" << std::endl;
	std::cout << "  sockaddr_in.sin_port = " << ntohs(addr.sin_port) << std::endl;
	std::cout << std::endl;
}

void	display_epoll_event(const struct epoll_event& event, const std::string& msg)
{
	std::cout << "[epoll_event] " << msg << std::endl;
	// std::cout << "  event.data.ptr = " << event.data.ptr << std::endl;
	std::cout << "  event.data.fd = " << event.data.fd << std::endl;
	// std::cout << "  event.data.u32 = " << event.data.u32 << std::endl;
	// std::cout << "  event.data.u64 = " << event.data.u64 << std::endl;

	std::cout << "  ";
	display_bits(event.events);

	if ((event.events & EPOLLIN) != 0)
		std::cout << "  EPOLLIN" << std::endl;

	if ((event.events & EPOLLOUT) != 0)
		std::cout << "  EPOLLOUT" << std::endl;

	if ((event.events & EPOLLRDHUP) != 0)
		std::cout << "  EPOLLRDHUP" << std::endl;

	if ((event.events & EPOLLHUP) != 0)
		std::cout << "  EPOLLHUP" << std::endl;

	if ((event.events & EPOLLERR) != 0)
		std::cout << "  EPOLLERR" << std::endl;

	if ((event.events & EPOLLET) != 0)
		std::cout << "  EPOLLET" << std::endl;

	std::cout << std::endl;
}

void	send_example_page(int client_fd)
{
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

	std::stringstream response;
	response << "HTTP/1.1 200 OK\r\nContent-Length: " << body.length() << "\r\n\r\n";
	response << body;

	send(client_fd, response.str().c_str(), response.str().length(), 0);
}