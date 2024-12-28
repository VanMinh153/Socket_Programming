#include "include_all.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// run: ./server <PORT> <ADMIN_PORT>
int main(int argc, char *argv[]) {
  int PORT = SERVER_PORT;
  if (argc == 2) {
    PORT = atoi(argv[1]);
  }
  memset(accounts, 0, sizeof(accounts));
  memset(sessions, 0, sizeof(sessions));

// init server
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Error creating socket");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("Error binding socket");
    return 1;
  }

  listen(server_socket, MAX_CLIENTS);
  printf("Server running on port %d\n", PORT);

  db_read_accounts();

  while (1) {
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                           &client_addr_len);
    if (client_socket < 0) {
      perror("Error accepting connection");
      continue;
    }

    if (session_count >= MAX_CLIENTS) {
      send(client_socket, "MAX CLIENTS REACHED");
      close(client_socket);
    }

    sessions[session_count].socket = client_socket;
    sessions[session_count].is_authenticated = 0;
    session_count++;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client,
                   &sessions[session_count - 1]);
    pthread_detach(thread_id);
  }

  close(server_socket);
  return 0;
}