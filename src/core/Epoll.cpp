#include "core/Epoll.hpp"

Epoll::Epoll():
	_fd(-1)
{
	_fd = epoll_create1(0);
	if (_fd < 0)
	{
		throw std::runtime_error("epoll_create1() failed");
	}

	std::cout << MAG << "[Epoll] Started epoll on fd: " << _fd << CRESET << std::endl;
}

Epoll::~Epoll()
{
	if (_fd != -1)
	{
		std::cout << MAG << "[Epoll] CLOSING fd: " << _fd << CRESET << std::endl;
		close(_fd);
	}
}





void	Epoll::add_fd(int fd_to_add, uint32_t flags)
{
	struct epoll_event  event;
	std::memset(&event, 0, sizeof(event));
	event.events = flags;
	event.data.fd = fd_to_add;

	std::cout << MAG << "[Epoll] Adding fd " << fd_to_add << CRESET << std::endl;
	if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd_to_add, &event) < 0)
	{
		throw std::runtime_error("epoll_ctl(EPOLL_CTL_ADD) failed");
	}
}

void	Epoll::remove_fd(int fd_to_rm)
{
	std::cout << MAG << "[Epoll] Removing fd " << fd_to_rm << CRESET << std::endl;
	if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd_to_rm, NULL) < 0)
	{
		throw std::runtime_error("epoll_ctl(EPOLL_CTL_DEL) failed");
	}
}

void	Epoll::wait(struct epoll_event* event, int& nfds)
{
	std::cout << MAG << "[Epoll] Waiting..." << CRESET << std::endl;
	nfds = epoll_wait(_fd, event, EPOLL_SIZE, -1);
	std::cout << MAG << "[Epoll] Event !" << CRESET << std::endl;
}