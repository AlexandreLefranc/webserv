#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <cstring>
#include <iostream>

#include "webserv.hpp"

struct  VirtualServer
{
    int     fd;
    int     family;
    int     addr;
    int     port;

    bool    initiated;
    bool    binded;
    bool    listening;

    VirtualServer();
    ~VirtualServer();

    void    init();
    int     accept_client();
};

#endif