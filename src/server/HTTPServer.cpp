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

void	HTTPServer::_add_server_to_epoll(const VirtualServer& server)
{
    _epoll.add_fd(server.fd, EPOLLIN | EPOLLET);
}

void	HTTPServer::run()
{
    _add_server_to_epoll(_server);

    struct epoll_event  event;
    _epoll.wait(event);
}