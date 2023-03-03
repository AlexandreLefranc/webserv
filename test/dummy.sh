#!/bin/sh

echo -n "GET / HTTP/1.1\r\nHost: bob.com\r\n\r\n"

echo -n "GET / HTTP/1.1\r\nHost:example.com\r\nConnection: close\r\n\r\n"


echo -n "GET / HTTP/1.1\r\nHost:_\r\nContent-Length:20\r\n\r\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

echo -n "GET /index.php?a=b&c=d HTTP/1.1\r\nhost:_\r\ncontent-length:20\r\n\r\n012345678901234567890123456789"

echo -n "GET / HTTP/1.1\r\nHost:_\r\ntransfer-encoding:chunked\r\n\r\n0\r\n\r\n"
echo -n "GET / HTTP/1.1\r\nHost:_\r\ntransfer-encoding:chunked\r\n\r\nA\r\n0123456789\r\n5\r\n01234\r\n0\r\n\r\n"