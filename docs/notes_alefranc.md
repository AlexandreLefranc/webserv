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

- getaddrinfo: malloc a linked list
- freeaddrinfo: free the linked list
- gai_strerror: strerror for getaddrinfo and freeaddrinfo

- getprotobyname: get protocol details. malloced structure
- fcntl: low level fd manipulator

# Notes RFC 9110

## 1.3.

Client constructs requests.

Server listens for requests, parses, interprets and responds one or more response messages.

## 3. Terminology

- Resources: URI
- Representations
- Client: Connect to server and send HTTP requests
- Server: Accept connections and send HTTP responses
- Messages: 
	- Request message: method + target (+ header fields)
	- Response message: Status code ( + header fields + metadata + content)
- User Agents: Client program that initiate a request (browser, cli, spiders, ...)
- Origin Server: Program that give respond to a given target resource
- Upstream/Downstream: Messages flow from upstream to downstream
- Inbound: Toward Origin Server
- Outbound: Toward User Agent
- Intermediaries:
	- proxy: message-forwarding agent that is chosen by the client. Message can be modified
	- gateway (aka reverse proxy): Acts as an Origin Server for outbound connection but translates received requests and forwards them inbound to another server
	- tunnel: blind relay between 2 connections without changing the message
- Caches: Local store of previous response messages

## 4. Identifiers in HTTP

## 5. Fields

HTTP uses fields to provide data within header or trailer sections of messages. case-insensitive

Order matters

If Server receive a too long field line, server must reply with 4xx status code

Client may discard too long field line

CTL characters are invalid => reject message

Comma is used as delimiter between members

...

## 6. Message Abstraction

A message consists of:

- control data to describe and route the message
- header
- content
- trailer

Control Data:

- Requests message includes Method, Target, Protocol version
- Response message includes status code, optional reason phrase, Protocol version

...

## 7. Routing HTTP Messages