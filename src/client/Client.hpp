#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

#include "webserv.hpp"

struct Client
{
    int     fd;
    int     associated_server_fd;

    struct sockaddr_in  sockaddr;
    socklen_t           socklen;


    Client();
    ~Client();

    void    accept_conection(int server_fd);
    void    clear();
};

#endif