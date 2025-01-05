#ifndef CLIENT_H
#define CLIENT_H

#include "include_all.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FAIL_TO_SEND_MSG "Fail to send message to server\n"
#ifdef TEST
#define TEST_APP_TERMINAL
#endif

int print_msg(char *msg);

#endif