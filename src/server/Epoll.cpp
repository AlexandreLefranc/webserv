#include "server/Epoll.hpp"

Epoll::Epoll():
	_fd(-1), _initiated(false)
{}

Epoll::~Epoll()
{
	if (_initiated == true)
	{
		std::cout << MAG << "[Epoll] CLOSING fd: " << _fd << CRESET << std::endl;
		close(_fd);
	}
}

void	Epoll::init()
{
	std::cout << MAG << "[Epoll] Init..." << CRESET << std::endl;
	std::cout << MAG << "[Epoll] OPENING fd: " << CRESET;
	_fd = epoll_create1(0);
	if (_fd < 0)
	{
		throw std::runtime_error("epoll_create1() failed");
	}
	std::cout << MAG << _fd << CRESET << std::endl;
	_initiated = true;
}

void	Epoll::add_fd(int fd_to_add, uint32_t flags)
{
	struct epoll_event  event;
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

void	Epoll::wait(struct epoll_event& event, int& nfds)
{
	std::cout << MAG << "[Epoll] Waiting..." << CRESET << std::endl;
	nfds = epoll_wait(_fd, &event, 1, -1);
	std::cout << MAG << "[Epoll] Event !" << CRESET << std::endl;
}

int		Epoll::fd() const
{
	return _fd;
}