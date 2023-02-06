#include <sys/types.h> // for portability
#include <sys/socket.h> // socket, bind, listen, getsockname
#include <unistd.h> // close
#include <arpa/inet.h> // struct sockaddr_in
#include <string.h> // memset
#include <sys/epoll.h> // epoll_*

#include <iostream>
#include <vector>
#include <string>

#define BUFF_SIZE 1024

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
		return 1;
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
		return 1;
	}

	// Listening to addr:port is on 
	ret = listen(server_fd, 42);
	if (ret < 0)
	{
		std::cout << "Listen failed" << std::endl;
		return 1;
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

static bool	is_ready_for_reading(const struct epoll_event& event)
{
	if ((event.events & EPOLLIN) != 0)
		return true;
	return false;
}

int		main()
{
	int epoll_fd = epoll_init();
	int server_fd = init_server();
	
	// Add server_fd to monitor list
	add_fd_to_epoll(epoll_fd, server_fd, EPOLLIN | EPOLLOUT | EPOLLET);
	
	int nfds = 0;
	struct epoll_event event;
	int i = 0;
	// char* str[BUFF_SIZE];

	std::vector<int> client_fds;

	int client_fd;
	struct sockaddr_in	client_sockaddr;
	memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t client_socklen = sizeof(client_sockaddr);
	while (true)
	{
		std::cout << "----------------------------------------------------------" << std::endl;
		std::cout << "epoll is waiting..." << i << std::endl;
		nfds = epoll_wait(epoll_fd, &event, 1, -1);
		std::cout << "epoll has detected some change in fd" << std::endl;
		for (int j = 0; j < nfds; j++)
		{
			display_epoll_event(event);

			if (event.data.fd == server_fd)
			{
				std::cout << "Accepting a client" << std::endl;
				client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
				if (client_fd < 0)
				{
					std::cout << "Accept failed" << std::endl;
					return 1;
				}
				std::cout << "Accept has accepted a connection ! client_fd = " << client_fd << std::endl << std::endl;
				display_sockaddr_in(client_sockaddr);
				client_fds.push_back(client_fd);

				add_fd_to_epoll(epoll_fd, client_fd, EPOLLIN | EPOLLET);
			}
			else
			{
				std::cout << "Other event" << std::endl;
				if (is_ready_for_reading(event) == true)
				{
					std::cout << "Ici je lis mdr" << std::endl;
				}
			}
		}

		i++;
	}






	close(epoll_fd);
	// close(client_fd);
	close(server_fd);

	return 0;
}