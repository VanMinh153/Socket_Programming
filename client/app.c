// #include "client.h"
// #include "view.h"
// #include "controller.h"

// int main() {

//   return 0;
// }
#include "config.h"
#include "tcp_socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define recv(fd, buf) recv_msg((fd), (buf), client_buffer)
char client_buffer[MSG_SIZE*10] = {0};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <IP_ADDRESS> <PORT>\n", argv[0]);
    return 1;
  }

  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[MSG_SIZE];

  // Create socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("Error creating socket");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
    perror("Invalid IP address");
    return 1;
  }

  // Connect
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0) {
    perror("Error connecting to server");
    return 1;
  }

  int authenticated = 0;
  while (1) {
    while (authenticated == 0) {
      printf("Choose an option:\n");
      printf("1. Login\n");
      printf("2. Register\n");
      printf("Enter choice (1-2): ");

      int choice;
      if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n')
          ; // Clear input buffer
        printf("Invalid input. Try again.\n");
        continue;
      }
      getchar(); // Consume newline

      char username[50], password[50];
      printf("Enter username: ");
      fgets(username, sizeof(username), stdin);
      username[strcspn(username, "\n")] = 0;

      printf("Enter password: ");
      fgets(password, sizeof(password), stdin);
      password[strcspn(password, "\n")] = 0;

      char message[MSG_SIZE];
      if (choice == 1) {
        snprintf(message, sizeof(message), "LOGIN %s %s", username, password);
      } else if (choice == 2) {
        snprintf(message, sizeof(message), "REGISTER %s %s", username,
                 password);
      } else {
        printf("Invalid choice. Try again.\n");
        continue;
      }

      send(client_socket, message);

      memset(buffer, 0, MSG_SIZE);
      recv(client_socket, buffer);
      // printf("@@ recv()\n");

      if (strstr(buffer, "SUCCESS")) {
        authenticated = 1;
        if (choice == 1) {
          printf("Login successful!\n");
        } else {
          printf("Registration successful!\n");
        }
      } else {
        printf("Failed. Please try again.\n");
      }
    }

    printf("\nStart chatting. Type 'exit' to quit the chat and sign out.\n");
// Chat room
//________________________________________________________________________________________
    while (1) {
      // Send message
      printf("<type your message>: ");
      fgets(buffer, MSG_SIZE, stdin);
      // Remove trailing newline
      buffer[strlen(buffer) - 1] = 0;

      if (strcmp(buffer, "exit") == 0) {
        send(client_socket, "EXIT");
        authenticated = 0;
        break;
      }

      if (strlen(buffer) > 0) {
        send(client_socket, buffer);
      }

      // Receive message
      recv(client_socket, buffer);
      printf("%s\n", buffer);
      while (getmsg(buffer, client_buffer)) {
        printf("%s\n", buffer);
      }
    }
  }

  close(client_socket);
  return 0;
}