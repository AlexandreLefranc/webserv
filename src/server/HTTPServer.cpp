#include "server/HTTPServer.hpp"

HTTPServer::HTTPServer(const std::string& confg_file):
    _config_file(confg_file)
{
    _parse_config();
    _epoll.init();
    _server.init();
}

HTTPServer::~HTTPServer()
{
    // std::cout << "HTTPServer destructor" << std::endl;
}

void	HTTPServer::_parse_config()
{
    std::cout << "Parsing config..." << std::endl;
}

void	HTTPServer::run()
{
    _epoll.add_fd(_server.fd, EPOLLIN | EPOLLET);

    struct epoll_event  event;
    int                 nfds;
    while (true)
    {
        _epoll.wait(event, nfds);

        std::cout << "Received " << nfds << " events" << std::endl;
        for (int i = 0; i < nfds; i++)
        {
            if (event.data.fd == _server.fd)
            {
                std::cout << "Connection request!" << std::endl;
                int client_fd = _server.accept_client();
                _epoll.add_fd(client_fd, EPOLLIN | EPOLLET);
            }
            else
            {
                std::cout << "Client is talking !" << std::endl;
                if ((event.events & EPOLLIN) != 0)
                {
                    char    str[1024];
                    ssize_t ret = recv(event.data.fd, str, 1023, 0);
                    str[ret] = '\0';
                    std::cout << str;
                }
                send(event.data.fd, "GOT IT !\n", 10, MSG_NOSIGNAL);
                _epoll.remove_fd(event.data.fd);
                close(event.data.fd);
            }
        }

    }
}