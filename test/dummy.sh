#!/bin/sh

echo -n "GET / HTTP/1.1\r\nHost: bob.com\r\n\r\n"

echo -n "GET / HTTP/1.1\r\nHost:example.com\r\nConnection: close\r\n\r\n"


echo -n "GET / HTTP/1.1\r\nHost:_\r\nContent-Length:20\r\n\r\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"