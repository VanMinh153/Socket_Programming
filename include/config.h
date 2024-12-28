#ifndef CONFIG_H
#define CONFIG_H

#define SERVER_PORT 5050
#define ADMIN_SERVER_PORT 5051

#define MAX_CLIENTS 1000
#define LEN_MSG 1024
#define SIZE_BUFFER_MSG LEN_MSG

#define DB_ACCOUNTS "accounts.txt"
#define DB_EVENTS "events.txt"

//***
#include "tcp_socket.h"
#define send(fd, buf) send_msg((fd), (buf))

#endif
