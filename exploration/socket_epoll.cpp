#include <sys/types.h> // for portability
#include <sys/socket.h> // socket, bind, listen, getsockname
#include <unistd.h> // close
#include <arpa/inet.h> // struct sockaddr_in
#include <string.h> // memset
#include <sys/epoll.h> // epoll_*

#include <iostream>

#define BUFF_SIZE 1024

static void	display_epoll_event(const struct epoll_event& event)
{
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

int		main()
{
	int epoll_fd = epoll_init();
	int server_fd = init_server();

	// // Init client info
	// struct sockaddr_in	client_sockaddr;
	// memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
	// socklen_t client_socklen = sizeof(client_sockaddr);

	// // Accept a client
	// std::cout << "Accept is blocking !" << std::endl;
	// int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
	// if (client_fd < 0)
	// {
	// 	std::cout << "Accept failed" << std::endl;
	// 	return 1;
	// }
	// std::cout << "Accept has accepted a connection ! client_fd = " << client_fd << std::endl << std::endl;

	struct epoll_event event;
	// Add server_fd to monitor list
	event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	event.data.fd = server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0)
	{
		std::cerr << "epoll_ctl failed\n";
		return 1;
	}

	// // Add file_fd to monitoring list
	// event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	// event.data.fd = client_fd;
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0)
	// {
	// 	std::cerr << "epoll_ctl failed\n";
	// 	return 1;
	// }

	int nfds = 0;
	int i = 0;
	while (i < 5)
	{
		std::cout << "epoll is waiting..." << i << std::endl;
		nfds = epoll_wait(epoll_fd, &event, 1, -1);
		std::cout << "epoll has detected some change in file" << std::endl;

		display_epoll_event(event);

		i++;
	}






	close(epoll_fd);
	// close(client_fd);
	close(server_fd);

	return 0;
}