project_structure/
├── Makefile
├── src/
│   ├── main.c
│   ├── server.c
│   ├── server.h
│   ├── client.c
│   ├── client.h
│   ├── database.c
│   ├── database.h
│   ├── event.c
│   ├── event.h
│   ├── user.c
│   ├── user.h
│   ├── logger.c
│   ├── logger.h
│   └── utils.c
│   └── utils.h
└── include/
    └── (thư viện bổ sung nếu cần)

// main.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "logger.h"

#define SERVER_PORT 8080
#define MAX_CONNECTIONS 100

pthread_t connection_thread, request_thread;

int main() {
    init_logger("event_app.log");
    
    // Khởi tạo socket server
    int server_socket = init_server(SERVER_PORT);
    if (server_socket == -1) {
        log_error("Không thể khởi tạo server");
        return EXIT_FAILURE;
    }

    // Tạo thread quản lý kết nối
    if (pthread_create(&connection_thread, NULL, handle_connections, &server_socket) != 0) {
        log_error("Không thể tạo connection thread");
        return EXIT_FAILURE;
    }

    // Tạo thread xử lý yêu cầu
    if (pthread_create(&request_thread, NULL, process_client_requests, NULL) != 0) {
        log_error("Không thể tạo request thread");
        return EXIT_FAILURE;
    }

    // Đợi các thread hoàn thành
    pthread_join(connection_thread, NULL);
    pthread_join(request_thread, NULL);

    close_logger();
    return EXIT_SUCCESS;
}

// server.h
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

// server.c
#include "server.h"
#include "user.h"
#include "event.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

client_info clients[MAX_CLIENTS];
struct pollfd poll_fds[MAX_CLIENTS];
int num_clients = 0;

int init_server(int port) {
    int server_socket;
    struct sockaddr_in server_address;

    // Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        log_error("Không thể tạo socket");
        return -1;
    }

    // Cấu hình địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Liên kết socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        log_error("Liên kết socket thất bại");
        return -1;
    }

    // Lắng nghe kết nối
    if (listen(server_socket, 5) < 0) {
        log_error("Không thể lắng nghe kết nối");
        return -1;
    }

    return server_socket;
}

void* handle_connections(void* arg) {
    int server_socket = *(int*)arg;
    struct sockaddr_in client_address;
    socklen_t client_addr_len = sizeof(client_address);

    while (1) {
        // Chấp nhận kết nối mới
        int new_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        if (new_socket < 0) {
            log_error("Chấp nhận kết nối thất bại");
            continue;
        }

        // Thêm client mới vào danh sách
        if (num_clients < MAX_CLIENTS) {
            clients[num_clients].socket_fd = new_socket;
            clients[num_clients].is_authenticated = 0;
            memset(clients[num_clients].username, 0, sizeof(clients[num_clients].username));

            poll_fds[num_clients].fd = new_socket;
            poll_fds[num_clients].events = POLLIN;

            num_clients++;
            log_info("Kết nối mới từ client");
        } else {
            log_error("Vượt quá số lượng client tối đa");
            close(new_socket);
        }
    }

    return NULL;
}

void* process_client_requests(void* arg) {
    while (1) {
        int ready_count = poll(poll_fds, num_clients, POLL_TIMEOUT);
        if (ready_count < 0) {
            log_error("Lỗi poll()");
            continue;
        }

        for (int i = 0; i < num_clients; i++) {
            if (poll_fds[i].revents & POLLIN) {
                char buffer[1024];
                int bytes_received = recv(clients[i].socket_fd, buffer, sizeof(buffer), 0);

                if (bytes_received <= 0) {
                    // Đóng kết nối nếu client ngắt
                    close(clients[i].socket_fd);
                    poll_fds[i] = poll_fds[num_clients - 1];
                    clients[i] = clients[num_clients - 1];
                    num_clients--;
                } else {
                    buffer[bytes_received] = '\0';
                    handle_client_message(&clients[i], buffer);
                }
            }
        }
    }
}

void handle_client_message(client_info* client, char* message) {
    // Xử lý các loại message từ client
    char* delimiter = "\r\n";
    char* token = strtok(message, delimiter);

    while (token != NULL) {
        // Phân tích và xử lý từng token
        if (strncmp(token, "LOGIN:", 6) == 0) {
            // Xử lý đăng nhập
            handle_login(client, token + 6);
        } else if (strncmp(token, "REGISTER:", 9) == 0) {
            // Xử lý đăng ký
            handle_registration(client, token + 9);
        } else if (strncmp(token, "CREATE_EVENT:", 13) == 0) {
            // Xử lý tạo sự kiện
            handle_create_event(client, token + 13);
        }
        // Thêm các xử lý khác cho các loại message

        token = strtok(NULL, delimiter);
    }
}

// user.h
#ifndef USER_H
#define USER_H

#include "server.h"

typedef struct {
    char username[50];
    char password[50];
    char* friends[100];
    int friend_count;
} User;

int handle_login(client_info* client, char* credentials);
int handle_registration(client_info* client, char* user_info);
int send_friend_request(const char* from_user, const char* to_user);
int accept_friend_request(const char* from_user, const char* to_user);
int remove_friend(const char* user, const char* friend_to_remove);

#endif

// event.h
#ifndef EVENT_H
#define EVENT_H

#include <time.h>

typedef enum {
    PRIVATE,
    PUBLIC
} EventType;

typedef struct {
    char name[100];
    time_t timestamp;
    char location[100];
    EventType type;
    char description[500];
    char* invited_users[100];
    char* participated_users[100];
    int invited_count;
    int participated_count;
} Event;

int handle_create_event(client_info* client, char* event_details);
int list_events(client_info* client);
int update_event(client_info* client, char* event_details);
int delete_event(client_info* client, int event_id);
int invite_to_event(const char* event_name, const char* inviter, const char* invited_user);
int request_event_participation(const char* event_name, const char* requester);

#endif

// logger.h
#ifndef LOGGER_H
#define LOGGER_H

void init_logger(const char* log_file);
void log_info(const char* message);
void log_error(const char* message);
void close_logger();

#endif

// Makefile
CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lpthread

TARGET = event_management_app
SOURCES = src/main.c src/server.c src/user.c src/event.c src/logger.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(TARGET)