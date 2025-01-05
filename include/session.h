#ifndef SESSION_H
#define SESSION_H

#include "config.h"
#include "account.h"
#include <stdbool.h>

#ifdef TEST
// #define TEST_SESSION
#endif

typedef struct {
    char user_id;
    int connfd;
    char recv_buffer[MSG_BUFFER_SIZE];
} session_t;

extern session_t sessions[MAX_SESSIONS];
extern int count_sessions;

// int create_session(int connfd);
// int delete_session(int connfd);

#endif