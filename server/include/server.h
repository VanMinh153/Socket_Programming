#ifndef SERVER_H
#define SERVER_H

#include "include_all.h"
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
#define LOGGED_IN "Have been logged in"
#define NOT_LOGGED_IN "Have not logged in"
#define ACCOUNT_NOT_EXIST "Account does not exist"
#define ACCOUNT_LOCKED "Account is locked"

#define CONNECT_SUCCESS "Connect successful"
#define LOGIN_SUCCESS "Login successful"
#define LOGOUT_SUCCESS "Logout successful"
#define REGISTER_SUCCESS "Register successful"
#define CHANGE_PASSWORD_SUCCESS "Change password successful"
#define FEEDBACK_SUCCESS "Feedback successful"
#define GET_IP_SUCCESS "Get IP address successful"
#define GET_DOMAIN_SUCCESS "Get domain name successful"
#define UPLOAD_SUCCESS "Upload successful"

#define PASSWORD_INCORRECT "Password is incorrect"
#define ACCOUNT_EXISTED "Account existed"
#define INVALID_IP "Invalid IP address"
#define NOT_FOUND "Not found"
#define GET_IP_FAIL "IP address not found"
#define GET_DOMAIN_FAIL "Domain name not found"
#define UPLOAD_READY "Server is ready to receive file"
#define FILE_TOO_LARGE "File too large"

#define MSG_NOT_DETERMINED "Message cannot determined"
#define MSG_OVERLENGTH "Message exceed the maximum message size"
#define RECV_DATA_FAIL "Get data fail"
#define INVALID_COMMAND "Invalid command"
#define INVALID_ARGS "Invalid arguments"

#define SERVER_ERROR "Server error"
//______________________________________________________________________

int init_server(int port);
void* handle_connections(void* arg);
void* process_client_requests(void* arg);
void handle_client_message(session_t* session, char* message);

#endif