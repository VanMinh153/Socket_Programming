#ifndef CONFIG_H
#define CONFIG_H

#include "tcp_socket.h"

#define send(fd, buf) send_msg((fd), (buf))

#endif