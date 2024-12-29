#include "include/server.h"

// run: ./server <PORT> <ADMIN_PORT>
int main(int argc, char *argv[]) {
  int PORT = SERVER_PORT;
  if (argc == 2) {
    PORT = atoi(argv[1]);
  }
  memset(accounts, 0, sizeof(accounts));
  memset(sessions, 0, sizeof(sessions));

  // init server
  int serverfd, connfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd < 0) {
    perror("Error creating socket");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Error binding socket");
    return 1;
  }

  listen(serverfd, MAX_SESSIONS);
  printf("Server running on port %d\n", PORT);

  db_read_accounts();
  printf("Loaded %d accounts from database\n", count_accounts);
  db_read_events();
  printf("Loaded %d events from database\n", count_events);

  int retval = -1;
  char client_msg[LEN_MSG];

  struct pollfd poll_listenfd;
  poll_listenfd.fd = serverfd;
  poll_listenfd.events = POLLIN;

  struct pollfd pollfds[MAX_FDS];
  for (int i = 0; i < MAX_FDS; i++) {
    pollfds[i].fd = -1;
    pollfds[i].events = POLLIN;
  }
  int nready = 0;

  while (1) {
    while (1) {
      retval = poll(&poll_listenfd, 1, TIME_OUT);
      if (retval == -1) {
        perror("\npoll():poll_listenfd");
        return -1;
      }
      if ((poll_listenfd.revents & POLLIN) == 0) {
        break;
      }

      if (count_sessions == MAX_SESSIONS) {
        fprintf(stderr, "Number of connections exceeds the capacity (%d)\n",
                MAX_SESSIONS);
        break;
      }

      connfd =
          accept(serverfd, (struct sockaddr *)&client_addr, &client_addr_len);
      if (connfd == -1) {
        perror("\naccept()");
        return -1;
      }
      if (create_session(connfd) != 0) {
        fprintf(stderr, "Error: create_session()\n");
        return -1;
      }
      pollfds[connfd].fd = connfd;
      send_msg(connfd, CONNECT_SUCCESS);
    }

    // Get message and response to client
    nready = poll(pollfds, MAX_FDS, TIME_OUT);

    if (nready == 0)
      continue;
    else if (nready == -1) {
      perror("\npoll():pollfds");
      continue;
    }

    for (int i = 0; i < MAX_FDS; i++) {
      if (pollfds[i].fd == -1 || (pollfds[i].revents & POLLIN) == 0)
        continue;

      connfd = i;
      while (1) {
        retval = recv_msg(connfd, client_msg, sessions[i].recv_buffer);
        if (retval > 0) {
          handle_msg(client_msg, sessions[i]);
        } else {
          if (retval == -3) {
            fprintf(stderr,
                    "Warning: message length is limited to %d characters\n",
                    LEN_MSG);
            send_msg(MSG_NOT_DETERMINED, connfd);
            break;
          } else if (retval == -2) {
            close(connfd);
            printf("[%d:%s]: (Disconnected)\n", connfd,
                   sessions[connfd].username[connfd] == NULL
                       ? ""
                       : sessions[connfd].username[connfd]);
            delete_session(connfd);
            pollfds[connfd].fd = -1;
            count_sessions--;
            break;
          } else {
            // recv() error
            // return -1;
            fprintf(stderr, "Can not get message from \"%s\"'s client\n",
                    sessions[connfd].username);
            break;
          }
        }

        if (strstr(sessions[i].recv_buffer[0], DELIMITER) != NULL)
          continue;

        if (poll(pollfds + connfd, 1, 0) > 0)
          if ((pollfds[i].revents & POLLIN) != 0)
            continue;

        break;
      }

      nready--;
      if (nready == 0)
        break;
    }
  }
  close(serverfd);
  return 0;
}