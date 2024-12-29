#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <arpa/inet.h>
#include <config.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdbool.h>

#ifdef TEST
#define TEST_TCP_SOCKET_1
#define TEST_TCP_SOCKET_2
#endif

bool check_connection();
int _send(int fd, void *buf, size_t n, int flags);
int _recv(int fd, void *buf, size_t n, int flags);
int send_msg(int fd, char *msg);
int recv_msg(int fd, char *msg, char *msg_buf);
int getmsg(char *msg, char *msg_buf);
int getmsg_2(char *msg, char *recv_buf, char *msg_buf);

#endif