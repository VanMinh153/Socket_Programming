#ifndef SERVER_H
#define SERVER_H

#include "include_all.h"
#include <assert.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define SERVER_PORT 5555
#define TIME_OUT 3 // 3ms, timeout while call poll()
//______________________________________________________________________
#define CONNECT_SUCCESS "Connected to server"
#define OK "Success"

#define SIGNED_IN "Have been logged in"
#define NOT_SIGNED_IN "Have not logged in"
#define ACCOUNT_EXISTED "Account existed"
#define ACCOUNT_NOT_EXIST "Account does not exist"
#define PASSWORD_INCORRECT "Password is incorrect"
#define PASSWORD_FORMAT_INCORRECT "Password format is incorrect"
#define USERNAME_FORMAT_INCORRECT "Username format is incorrect"
#define USER_EXISTED "User existed"
#define USER_NOT_FOUND "User not found"
#define EVENT_EXISTED "Event existed"
#define EVENT_NOT_FOUND "Event not found"
#define REQUEST_EXISTED "Request existed"
#define REQUEST_NOT_FOUND "Request not found"
#define NOT_FRIEND "Not friend"

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

#define OUT_OF_MEMORY "Server database out of memory"
//______________________________________________________________________

#ifdef TEST
#define TEST_PARSE
#endif

#define send_(msg) send_msg(connfd, msg)
#define recv_(msg) recv_msg(connfd, msg, recv_buffer)

int handle_msg(char *msg, session_t *session);


#endif