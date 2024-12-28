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