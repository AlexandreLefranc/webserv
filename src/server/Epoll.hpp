#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>

#include "webserv.hpp"

class Epoll
{
private:
	int		_fd;
	bool	_initiated;

public:
	Epoll();
	~Epoll();

	void	init();
	void	add_fd(int fd_to_add, uint32_t flags);
	void	remove_fd(int fd_to_rm);
	void	wait(struct epoll_event& event, int& nfds);

	int		fd() const;
};

#endif