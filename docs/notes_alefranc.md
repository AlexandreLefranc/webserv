# Authorised functions

- All c++98 except Boost

- execve
- dup, dup2
- pipe
- fork

- strerror
- errno

- htons
- htonl
- ntohs
- ntohl

- select
- poll
- epoll, epoll_create, epoll_ctl, epoll_wait
- kqueue, kevent

- socket
- bind
- connect

- accept
- listen
- send
- recv

- setsockopt
- getsockname

- getaddrinfo
- freeaddrinfo
- gai_strerror: strerror for getaddrinfo and freeaddrinfo

- getprotobyname: get protocol details. malloced structure
- fcntl: low level fd manipulator