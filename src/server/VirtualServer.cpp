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

int VirtualServer::accept_client()
{
    int client_fd;
    struct sockaddr_in  client_sockaddr;
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));
    socklen_t               client_socklen = sizeof(struct sockaddr_in);

    std::cout << "Accepting a client" << std::endl;
    client_fd = accept(fd, reinterpret_cast<struct sockaddr*>(&client_sockaddr), &client_socklen);
    if (client_fd < 0)
    {
        throw std::runtime_error("accept() failed");
    }

    return client_fd;
}