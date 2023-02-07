#include "VirtualServer.hpp"

VirtualServer::VirtualServer():
    fd(-1), family(AF_INET), addr(INADDR_ANY), port(8080)
    , initiated(false), binded(false), listening(false)
{}

VirtualServer::~VirtualServer()
{
    if (fd != -1)
    {
        std::cout << MAG << "CLOSING server fd" << CRESET << std::endl;
        close(fd);
    }
}

void    VirtualServer::init()
{
    std::cout << "Init server..." << std::endl;
    std::cout << MAG << "OPENING server fd: " << CRESET;
    fd = socket(family, SOCK_STREAM, getprotobyname("tcp")->p_proto);
    if (fd == -1)
    {
        throw std::runtime_error("socket() failed");
    }
    std::cout << MAG << fd << CRESET << std::endl;

    struct sockaddr_in      sockaddr;
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    socklen_t               socklen = sizeof(struct sockaddr_in);
    sockaddr.sin_family = family;
    sockaddr.sin_addr.s_addr = htonl(addr);
    sockaddr.sin_port       = htons(port);

    if (bind(fd, reinterpret_cast<const struct sockaddr*>(&sockaddr), socklen) < 0)
    {
        throw std::runtime_error("bind() failed");
    }
    binded = true;

    if (listen(fd, 42) < 0)
    {
        throw std::runtime_error("listen() failed");
    }

    initiated = true;
}