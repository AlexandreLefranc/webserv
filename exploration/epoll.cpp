#include <sys/epoll.h>
#include <unistd.h> // close

#include <sys/stat.h>
#include <fcntl.h> // open

#include <errno.h>
#include <string.h>

#include <iostream>

int	main()
{
	// Create epoll instance
	int epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
	{
		std::cerr << "epoll_create1 failed\n";
		return 1;
	}

	// Open a file to monitor
	int file_fd = open("/mnt/nfs/homes/alefranc/Documents/cursus42/5_webserv/webserv/exploration/tmp", O_RDWR);
	if (file_fd < 0)
	{
		std::cerr << "open failed\n";
		return 1;
	}

	std::cout << "file_fd = " << file_fd << std::endl;

	// Add file_fd to monitoring list
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	event.data.fd = file_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, file_fd, &event) < 0)
	{
		std::cerr << "epoll_ctl failed\n";
		std::cerr << strerror(errno) << "\n";
		return errno;
	}

	int nfds = 0;
	int i = 0;
	while (i < 10)
	{
		std::cout << "epoll is waiting..." << std::endl;
		nfds = epoll_wait(epoll_fd, &event, 1, -1);
		std::cout << "epoll has detected some change in file" << std::endl;
		i++;
	}

	close(file_fd);
	close(epoll_fd);
	return 0;
}