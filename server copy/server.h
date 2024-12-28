#ifndef SERVER_H
#define SERVER_H

#include <poll.h>
#include <netinet/in.h>

#define MAX_CLIENTS 100
#define POLL_TIMEOUT 3000  // 3 giây

typedef struct {
    int socket_fd;
    char buffer[1024];
    int is_authenticated;
    char username[50];
} client_info;

int init_server(int port);
void* handle_connections(void* arg);
void* process_client_requests(void* arg);
void handle_client_message(client_info* client, char* message);

#endif