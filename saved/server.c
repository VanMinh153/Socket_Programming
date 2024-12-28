#include "server.h"
#include "account.h"
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