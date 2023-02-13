#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <sys/epoll.h>
# include <unistd.h>

# include <iostream>

# include "webserv.hpp"
# include "server/ServerManager.hpp"

// Purpose: Wrapper for epoll actions. 
// Manage epoll instance fd

class Epoll
{
private:
	int		_fd;

public:
	Epoll();
	~Epoll();

	void	init();
	void	add_fd(int fd_to_add, uint32_t flags);
	void	remove_fd(int fd_to_rm);
	void	wait(struct epoll_event* event, int& nfds);
};

#endif