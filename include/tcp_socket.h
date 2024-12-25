#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
// Functions for handling connections on ports 6000 and 6060

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// #define DEBUG_SEND_RECV
// #define DEBUG_SEND_RECV_2

#define MSG_SIZE 2048
#define DELIMITER "||"
#define DELIMITER_LEN 2

int send__(int fd, void *buf, size_t n, int flags);
int recv__(int fd, void *buf, size_t n, int flags);
int send_msg(int fd, char *msg);
int recv_msg(int fd, char *buf, char *msg_buf);
int getmsg(char *buf, char *msg_buf);
int getmsg_2(char *buf, char *recv_buf, char *msg_buf);

#endif