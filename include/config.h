#ifndef CONFIG_H
#define CONFIG_H

#define SERVER_PORT 5555

#define MAX_CLIENTS 1000
#define LEN_MSG 1024
#define SIZE_BUFFER_MSG LEN_MSG

#define DB_ACCOUNTS "database/accounts.txt"
#define DB_ACCOUNTS_BACKUP "database/accounts_backup.txt"
#define DB_EVENTS "database/events.txt"
#define MAX_LINE_TEXT 2048

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
//***
#include "tcp_socket.h"
#define send(fd, buf) send_msg((fd), (buf))

#endif
