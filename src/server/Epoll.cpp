#include "server/Epoll.hpp"

Epoll::Epoll():
    initiated(false), fd(-1)
{}

Epoll::~Epoll()
{
    if (initiated == true)
    {
        std::cout << MAG << "CLOSING epoll fd" << CRESET << std::endl;
        close(fd);
    }
}

void    Epoll::init()
{
    std::cout << "Init epoll..." << std::endl;
    std::cout << MAG << "OPENING epoll fd: " << CRESET;
    fd = epoll_create1(0);
	if (fd < 0)
	{
		throw std::runtime_error("epoll_create1() failed");
	}
    std::cout << MAG << fd << CRESET << std::endl;
    initiated = true;
}

void    Epoll::add_fd(int fd_to_add, uint32_t flags)
{
    struct epoll_event  event;
    event.events = flags;
    event.data.fd = fd_to_add;

    std::cout << "Adding fd " << fd_to_add << " to Epoll" << std::endl;
    if (epoll_ctl(fd, EPOLL_CTL_ADD, fd_to_add, &event) < 0)
    {
        throw std::runtime_error("epoll_ctl(EPOLL_CTL_ADD) failed");
    }
}

void    Epoll::remove_fd(int fd_to_rm)
{
    std::cout << "Removing fd " << fd_to_rm << " from Epoll" << std::endl;
    if (epoll_ctl(fd, EPOLL_CTL_DEL, fd_to_rm, NULL) < 0)
    {
        throw std::runtime_error("epoll_ctl(EPOLL_CTL_DEL) failed");
    }
}

void    Epoll::wait(struct epoll_event& event, int& nfds)
{
    std::cout << "epoll is waiting..." << std::endl;
    nfds = epoll_wait(fd, &event, 1, -1);
    std::cout << "epoll got something !" << std::endl;
}