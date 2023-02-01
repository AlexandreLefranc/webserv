#include <sys/types.h> // for portability
#include <sys/socket.h> // socket, bind, listen, getsockname
#include <unistd.h> // close
#include <arpa/inet.h> // struct sockaddr_in
#include <string.h> // memset

#include <iostream>

int main()
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

	std::cout << "server_sockaddr.sin_family = " << server_sockaddr.sin_family << std::endl;
	std::cout << "server_sockaddr.sin_addr.s_addr = " << ntohl(server_sockaddr.sin_addr.s_addr) << std::endl;
	std::cout << "server_sockaddr.sin_port = " << ntohs(server_sockaddr.sin_port) << std::endl << std::endl;


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

	// Init client info
	struct sockaddr_in	client_sockaddr;
	memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t client_socklen = sizeof(client_sockaddr);

	// Accept a client
	std::cout << "Accept is blocking !" << std::endl;
	int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
	if (client_fd < 0)
	{
		std::cout << "Accept failed" << std::endl;
		return 1;
	}
	std::cout << "Accept has accepted a connection ! client_fd = " << client_fd << std::endl << std::endl;

	std::cout << "client_sockaddr.sin_family = " << client_sockaddr.sin_family << std::endl;
	std::cout << "client_sockaddr.sin_addr.s_addr = " << ntohl(client_sockaddr.sin_addr.s_addr) << std::endl;
	std::cout << "which mean " << ((ntohl(client_sockaddr.sin_addr.s_addr) & 0xFF000000) >> 24) << "."
							   << ((ntohl(client_sockaddr.sin_addr.s_addr) & 0x00FF0000) >> 16) << "."
							   << ((ntohl(client_sockaddr.sin_addr.s_addr) & 0x0000FF00) >> 8) << "."
							   << ((ntohl(client_sockaddr.sin_addr.s_addr) & 0x000000FF) >> 0) << std::endl;
	std::cout << "client_sockaddr.sin_port = " << ntohs(client_sockaddr.sin_port) << std::endl << std::endl;


	// Retrieving socket info from socket_fd
	struct sockaddr_in	tmp_sockaddr;
	memset(&tmp_sockaddr, 0, sizeof(struct sockaddr_in));
	socklen_t tmp_socklen = sizeof(tmp_sockaddr);

	std::cout << "Create tmp_sockaddr clean" << std::endl;
	std::cout << "tmp_sockaddr.sin_family = " << tmp_sockaddr.sin_family << std::endl;
	std::cout << "tmp_sockaddr.sin_addr.s_addr = " << ntohl(tmp_sockaddr.sin_addr.s_addr) << std::endl;
	std::cout << "tmp_sockaddr.sin_port = " << ntohs(tmp_sockaddr.sin_port) << std::endl;

	std::cout << "Filling tmp_sockaddr with getsockname" << std::endl;
	getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&tmp_sockaddr), &tmp_socklen);
	std::cout << "tmp_sockaddr.sin_family = " << tmp_sockaddr.sin_family << std::endl;
	std::cout << "tmp_sockaddr.sin_addr.s_addr = " << ntohl(tmp_sockaddr.sin_addr.s_addr) << std::endl;
	std::cout << "tmp_sockaddr.sin_port = " << ntohs(tmp_sockaddr.sin_port) << std::endl;

	// Close server socket
	close(server_fd);
	close(client_fd);
	return 0;
}