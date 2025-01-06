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

//______________________________________________________________________

#ifdef TEST
#define TEST_PARSE
#endif

#define send_(msg) send_msg(connfd, msg)
#define recv_(msg) recv_msg(connfd, msg, recv_buffer)

int handle_msg(char *msg, session_t *session);


#endif