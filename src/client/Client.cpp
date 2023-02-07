#include "client/Client.hpp"

Client::Client():
    fd(-1), associated_server_fd(-1)
{
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    socklen = sizeof(struct sockaddr_in);
}

Client::~Client()
{
    if (fd != -1)
    {
        std::cout << YEL << "[Client] CLOSING fd: " << fd << CRESET << std::endl;
        close(fd);
    }
}

void    Client::accept_conection(int server_fd)
{
    std::cout << YEL << "[Client] Accept client" << CRESET << std::endl;
    std::cout << YEL << "[Client] OPENING fd: " << CRESET;
    fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
    if (fd < 0)
    {
        throw std::runtime_error("accept() failed");
    }
    std::cout << YEL << fd << CRESET << std::endl;

    associated_server_fd = server_fd;
}

void    Client::clear()
{
    if (fd != -1)
    {
        std::cout << YEL << "[Client] CLOSING fd: " << fd << CRESET << std::endl;
        close(fd);
        fd = -1;
    }
    associated_server_fd = -1;
}