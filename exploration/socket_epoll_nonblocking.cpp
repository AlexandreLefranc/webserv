#include <sys/types.h> // for portability
#include <sys/socket.h> // socket, bind, listen, getsockname
#include <unistd.h> // close
#include <arpa/inet.h> // struct sockaddr_in
#include <sys/epoll.h> // epoll_*

#include <cstring> // memset
#include <cerrno>

#include <iostream>
#include <vector>
#include <string>

#define BUFF_SIZE 32

static void	display_sockaddr_in(const struct sockaddr_in& addr, const std::string& msg = "")
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

static void	display_epoll_event(const struct epoll_event& event, const std::string& msg = "")
{
	std::cout << "[epoll_event] " << msg << std::endl;
	std::cout << "  event.data.ptr = " << event.data.ptr << std::endl;
	std::cout << "  event.data.fd = " << event.data.fd << std::endl;
	std::cout << "  event.data.u32 = " << event.data.u32 << std::endl;
	std::cout << "  event.data.u64 = " << event.data.u64 << std::endl;
	if ((event.events & EPOLLIN) != 0)
		std::cout << "  Ready for reading" << std::endl;
	if ((event.events & EPOLLOUT) != 0)
		std::cout << "  Ready for writing" << std::endl;
	std::cout << std::endl;
}

static int		epoll_init()
{
	// Create epoll instance
	int epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
	{
		std::cerr << "epoll_create1 failed\n";
		throw std::exception();
	}

	return epoll_fd;
}

static int		init_server()
{
	int ret;

	// Create server socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "server_fd = " << server_fd << std::endl;

	// Config socket
	struct sockaddr_in	server_sockaddr;
	memset(&server_sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t server_socklen = sizeof(server_sockaddr);
	server_sockaddr.sin_family = AF_INET; // IP v4
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP address
	server_sockaddr.sin_port = htons(8080); // port

	// Bind socket to config
	ret = bind(server_fd, reinterpret_cast<const struct sockaddr*>(&server_sockaddr), server_socklen);
	if (ret < 0)
	{
		std::cout << "Bind failed" << std::endl;
		throw std::exception();
	}

	// Listening to addr:port is on 
	ret = listen(server_fd, 42);
	if (ret < 0)
	{
		std::cout << "Listen failed" << std::endl;
		throw std::exception();
	}

	return server_fd;
}

static void	add_fd_to_epoll(int epoll_fd, int fd, uint32_t flags)
{
	struct epoll_event event;
	event.events = flags;
	event.data.fd = fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		std::cerr << "epoll_ctl failed" << std::endl;;
		throw std::exception();
	}
}

static void	accept_client_and_add_to_epoll(int epoll_fd, int server_fd)
{
	int					client_fd;
	struct sockaddr_in	client_sockaddr;
	memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t			client_socklen = sizeof(client_sockaddr);

	std::cout << "Accepting a client" << std::endl;
	client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
	if (client_fd < 0)
	{
		std::cout << "Accept failed" << std::endl;
		throw std::exception();
	}
	std::cout << "Accept has accepted a connection ! client_fd = " << client_fd << std::endl << std::endl;
	display_sockaddr_in(client_sockaddr);

	add_fd_to_epoll(epoll_fd, client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
}

static bool	is_ready_for_reading(const struct epoll_event& event)
{
	if ((event.events & EPOLLIN) != 0)
		return true;
	return false;
}

static void	print_socket(int fd)
{
	std::string	full("");
	char str[BUFF_SIZE];
	ssize_t ret;

	do
	{
		std::cout << "Reading socket: ";
		ret = recv(fd, str, BUFF_SIZE - 1, MSG_DONTWAIT);
		std::cout << ret << " bytes" << std::endl;
		if (ret > 0)
		{
			str[ret] = '\0';
			// std::cout << str;
			full += str;
		}
		// sleep(1);
	} while (ret > 0);

	std::cout << "Full message: " << full << std::endl;
}

int		main()
{
	int server_fd = init_server();
	int epoll_fd = epoll_init();
	
	add_fd_to_epoll(epoll_fd, server_fd, EPOLLIN | EPOLLET);

	struct epoll_event event;

	for (int i = 0;; i++)
	{
		std::cout << "----------------------------------------------------------" << std::endl;
		memset(&event, 0, sizeof(struct epoll_event));
		std::cout << "epoll is waiting..." << i << std::endl;
		int nfds = epoll_wait(epoll_fd, &event, 1, -1);
		std::cout << "epoll has detected some change in fd" << std::endl;
		for (int j = 0; j < nfds; j++)
		{
			display_epoll_event(event);

			if (event.data.fd == server_fd)
			{
				accept_client_and_add_to_epoll(epoll_fd, server_fd);
			}
			else
			{
				std::cout << "Event coming from a client: " << event.data.fd << std::endl;
				if (is_ready_for_reading(event) == true)
				{
					print_socket(event.data.fd);
				}
				if ((event.events & EPOLLOUT) != 0)
					std::cout << "  Ready for writing" << std::endl;
				if ((event.events & EPOLLRDHUP) != 0)
					std::cout << "  Client connection closed" << std::endl;
				std::cout << "Sending 'GOT IT !'" << std::endl;
				if (send(event.data.fd, "GOT IT !\n", 10, MSG_NOSIGNAL) < 0) // NOSIGNAL flag because ctrl-c in client kills server as there is a broken pipe
				{
					std::cout << "send failed: " << std::strerror(errno) << std::endl;
				}
			}
		}
	}






	close(epoll_fd);
	// close(client_fd);
	close(server_fd);

	return 0;
}