#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <iostream>

void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg = "")
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

void	display_epoll_event(const struct epoll_event& event, const std::string& msg = "")
{
	std::cout << "[epoll_event] " << msg << std::endl;
	std::cout << "  event.data.ptr = " << event.data.ptr << std::endl;
	std::cout << "  event.data.fd = " << event.data.fd << std::endl;
	std::cout << "  event.data.u32 = " << event.data.u32 << std::endl;
	std::cout << "  event.data.u64 = " << event.data.u64 << std::endl;
	if ((event.events & EPOLLIN) != 0)
		std::cout << "  EPOLLIN" << std::endl;
	if ((event.events & EPOLLOUT) != 0)
		std::cout << "  EPOLLOUT" << std::endl;
	if ((event.events & EPOLLRDHUP) != 0)
		std::cout << "  EPOLLRDHUP" << std::endl;
	std::cout << std::endl;
}