#craft custom request to the server
clear;printf "GET / HTTP/1.1\r\nHost: \z\r\nConnection: close\r\n\r\n" | nc 127.0.0.1 8082
