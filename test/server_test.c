// #include "config.h"
// #include "tcp_socket.h"
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <unistd.h>

// // #define MAX_CLIENTS 10
// #define MAX_USERNAME 50
// #define MAX_PASSWORD 50
// #define BUFFER_SIZE 1024
// #define recv(fd, buf) recv_msg((fd), (buf), server_buffer)

// typedef struct {
//   char username[MAX_USERNAME];
//   char password[MAX_PASSWORD];
// } Account;

// typedef struct {
//   int socket;
//   char username[MAX_USERNAME];
//   int is_authenticated;
// } SessionInfo;

// char server_buffer[MSG_SIZE * 10] = {0};
// Account accounts[MAX_CLIENTS];
// int account_count = 0;
// SessionInfo sessions[MAX_CLIENTS];
// int session_count = 0;

// pthread_mutex_t accounts_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// void load_accounts() { // Tested
//   FILE *file = fopen("accounts.txt", "r");
//   if (file == NULL) {
//     printf("No accounts file found. Creating new one.\n");
//     return;
//   }

//   pthread_mutex_lock(&accounts_mutex);
//   while (fscanf(file, "%s %s", accounts[account_count].username,
//                 accounts[account_count].password) == 2) {
//     account_count++;
//     if (account_count >= MAX_CLIENTS)
//       break;
//   }
//   pthread_mutex_unlock(&accounts_mutex);
//   fclose(file);
// }

// int authenticate_user(char *username, char *password) { // Tested
//   pthread_mutex_lock(&accounts_mutex);
//   for (int i = 0; i < account_count; i++) {
//     if (strcmp(accounts[i].username, username) == 0 &&
//         strcmp(accounts[i].password, password) == 0) {
//       pthread_mutex_unlock(&accounts_mutex);
//       return 1;
//     }
//   }
//   pthread_mutex_unlock(&accounts_mutex);
//   return 0;
// }

// int register_user(char *username, char *password) { // Tested
//   pthread_mutex_lock(&accounts_mutex);
//   // Check if username already exists
//   for (int i = 0; i < account_count; i++) {
//     if (strcmp(accounts[i].username, username) == 0) {
//       pthread_mutex_unlock(&accounts_mutex);
//       return 0;
//     }
//   }

//   // Add new account
//   strcpy(accounts[account_count].username, username);
//   strcpy(accounts[account_count].password, password);
//   account_count++;
//   // @database Save accounts to file
//   FILE *file = fopen("accounts.txt", "a");
//   if (file == NULL) {
//     printf("Error saving accounts file\n");
//     return 0;
//   }
//   fprintf(file, "%s %s\n", username, password);
//   fclose(file);

//   pthread_mutex_unlock(&accounts_mutex);
//   return 1;
// }

// void broadcast_message(char *message, char *sender) {
//   pthread_mutex_lock(&clients_mutex);
//   for (int i = 0; i < session_count; i++) {
//     if (strcmp(sessions[i].username, sender) != 0 &&
//         sessions[i].is_authenticated) {
//       send(sessions[i].socket, message);
//     }
//   }
//   pthread_mutex_unlock(&clients_mutex);
// }

// void *handle_client(void *arg) { // Tested
//   SessionInfo *client = (SessionInfo *)arg;
//   char buffer[BUFFER_SIZE];
//   int read_size;

//   while (1) {
//     // Login/Register process
//     while (client->is_authenticated == 0) {
//       memset(buffer, 0, BUFFER_SIZE);
//       read_size = recv(client->socket, buffer);
//       if (read_size <= 0) {
//         close(client->socket);
//         return NULL;
//       }

//       char command[10], username[MAX_USERNAME], password[MAX_PASSWORD];
//       if (sscanf(buffer, "%s %s %s", command, username, password) != 3) {
//         send(client->socket, "INVALID LOGIN/REGISTER FORMAT");
//         continue;
//       }

//       if (strcmp(command, "LOGIN") == 0) {
//         if (authenticate_user(username, password) == 0) {
//           send(client->socket, "FAILED");
//         } else {
//           strcpy(client->username, username);
//           client->is_authenticated = 1;
//           send(client->socket, "SUCCESS");
//           break;
//         }
//       } else if (strcmp(command, "REGISTER") == 0) {
//         if (register_user(username, password) == 0) {
//           send(client->socket, "FAILED");
//         } else {
//           strcpy(client->username, username);
//           send(client->socket, "SUCCESS");
//           client->is_authenticated = 1;
//           break;
//         }
//       }
//     }

//     // Chat process
//     while (1) {
//       read_size = recv(client->socket, buffer);
//       if (read_size <= 0) {
//         close(client->socket);
//         return NULL;
//       }

//       if (strcmp(buffer, "EXIT") == 0) {
//         client->is_authenticated = 0;
//         break;
//       }
//       char message[BUFFER_SIZE * 2];
//       snprintf(message, sizeof(message), "%s: %s", client->username, buffer);
//       broadcast_message(message, client->username);
//     }
//   }
//   return NULL;
// }
// //________________________________________________________________________________________
// int main(int argc, char *argv[]) {
//   if (argc != 2) {
//     printf("Usage: %s <PORT>\n", argv[0]);
//     return 1;
//   }
//   memset(accounts, 0, sizeof(accounts));
//   memset(sessions, 0, sizeof(sessions));

//   load_accounts();

//   int server_socket, client_socket;
//   struct sockaddr_in server_addr, client_addr;
//   socklen_t client_addr_len = sizeof(client_addr);
//   int port = atoi(argv[1]);

//   // Create socket
//   server_socket = socket(AF_INET, SOCK_STREAM, 0);
//   if (server_socket < 0) {
//     perror("Error creating socket");
//     return 1;
//   }

//   server_addr.sin_family = AF_INET;
//   server_addr.sin_addr.s_addr = INADDR_ANY;
//   server_addr.sin_port = htons(port);

//   // Bind
//   if (bind(server_socket, (struct sockaddr *)&server_addr,
//            sizeof(server_addr)) < 0) {
//     perror("Error binding socket");
//     return 1;
//   }

//   // Listen
//   listen(server_socket, MAX_CLIENTS);
//   printf("Server running on port %d\n", port);

//   while (1) {
//     client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
//                            &client_addr_len);
//     if (client_socket < 0) {
//       perror("Error accepting connection");
//       continue;
//     }

//     if (session_count >= MAX_CLIENTS) {
//       send(client_socket, "MAX CLIENTS REACHED");
//       close(client_socket);
//     }

//     sessions[session_count].socket = client_socket;
//     sessions[session_count].is_authenticated = 0;
//     session_count++;

//     pthread_t thread_id;
//     pthread_create(&thread_id, NULL, handle_client,
//                    &sessions[session_count - 1]);
//     pthread_detach(thread_id);
//   }

//   close(server_socket);
//   return 0;
// }