#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

int http_server_run(unsigned short port, const char* ip_address, int (*should_exit)(void));

#endif
