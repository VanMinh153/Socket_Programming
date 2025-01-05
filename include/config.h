#ifndef CONFIG_H
#define CONFIG_H
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define TEST

// tcp_socket.h
#define DELIMITER "||"
#define LEN_DELIMITER 2
#define LEN_MSG 2048
#define MSG_BUFFER_SIZE LEN_MSG + LEN_DELIMITER + 1 
#define LEN_COMMAND 15
#define LEN_OPTION 15 // include '-'
// session.h
#define MAX_SESSIONS 500
#define MAX_FDS MAX_SESSIONS + 10

// database
#define DB_ACCOUNTS "database/accounts.txt"
#define DB_ACCOUNTS_BACKUP "database/accounts_backup.txt"
#define DB_EVENTS "database/events.txt"
#define DB_EVENTS_BACKUP "database/events_backup.txt"
#define MAX_LINE_TEXT 2048

#endif
