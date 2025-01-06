#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "config.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#ifdef TEST
// #define TEST_TCP_SOCKET_1
#define TEST_TCP_SOCKET_2
#endif
//______________________________________________________________________
#define CONNECT_SUCCESS "Connected to server"
#define OK "Success"
#define OK2 "Success..."

#define SIGNED_IN "Have been logged in"
#define NOT_SIGNED_IN "Have not logged in"
#define ACCOUNT_EXISTED "Account existed"
#define ACCOUNT_NOT_EXIST "Account does not exist"
#define PASSWORD_INCORRECT "Password is incorrect"
#define USER_EXISTED "User existed"
#define USER_NOT_FOUND "User not found"
#define EVENT_EXISTED "Event existed"
#define EVENT_NOT_FOUND "Event not found"
#define REQUEST_EXISTED "Request existed"
#define REQUEST_NOT_FOUND "Request not found"
#define NOT_FRIEND "Not friend"


#define PASSWORD_INVALID "Password format is incorrect"
#define USERNAME_INVALID "Username format is incorrect"
#define EVENT_NAME_INVALID "Event name format is incorrect"
#define DATE_INVALID "Date format is incorrect"
#define ADDRESS_INVALID "Address format is incorrect"
#define TYPE_INVALID "Event type format is incorrect"
#define EVENT_DETAILS_INVALID "Event details format is incorrect"


#define COMMAND_NOT_FOUND "Command not found"
#define OPTION_INVALID "Invalid option"
#define OPTION_MISSING "Missing option"
#define OPTION_EXCEED "Exceed option"
#define ARGS_INVALID "Invalid arguments"
#define ARGS_MISSING "Missing arguments"
#define ARGS_EXCEED "Exceed arguments"
#define MSG_OVERLENGTH "Message exceed the maximum message size"
#define MSG_NOT_DETERMINED "Message cannot determined"
#define BLANK_MSG "Blank message"

#define NO_ACCESS "Do not have permission to access"
#define REACHED_LIMIT "Reached the limit of requests"
#define NOT_SUSSCESS "Not success"

#define OUT_OF_MEMORY "Server database out of memory"
//______________________________________________________________________

bool connection_is_active();
int send_(int fd, void *buf, size_t n, int flags);
int recv_(int fd, void *buf, size_t n, int flags);
int send_msg(int fd, char *msg);
int recv_msg(int fd, char *msg, char *msg_buf);
int getmsg(char *msg, char *msg_buf);
int getmsg_2(char *msg, char *recv_buf, char *msg_buf);

#endif