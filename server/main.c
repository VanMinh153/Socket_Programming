#include "include/server.h"

// run: ./server <PORT> <ADMIN_PORT>
int main(int argc, char *argv[]) {
  int PORT = SERVER_PORT;
  if (argc == 2) {
    PORT = atoi(argv[1]);
  }

  // init server
  int serverfd, connfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd == -1) {
    perror("\nsocket()");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("\nbind()");
    return 1;
  }

  listen(serverfd, MAX_SESSIONS);
  printf("Server running on port %d\n", PORT);
  //______________________________________________________________________

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
  //@@ OK

  while (1) {
    while (1) {
      retval = poll(&poll_listenfd, 1, TIME_OUT);
      if (retval == -1) {
        perror("\npoll(): poll_listenfd");
        return 1;
      }
      if ((poll_listenfd.revents & POLLIN) == 0)
        break;

      connfd =
          accept(serverfd, (struct sockaddr *)&client_addr, &client_addr_len);
      if (connfd == -1) {
        perror("\naccept()");
        return 1;
      }
      //@@
      printf("[%d]: (Connected)\n", connfd);
      sessions[connfd].connfd = connfd;
      count_sessions++;
      printf("Number of sessions: %d\n", count_sessions);

      pollfds[connfd].fd = connfd;
      send_msg(connfd, CONNECT_SUCCESS);
    }

    if (count_sessions == 0)
      continue;

    // Get message and response to client
    nready = poll(pollfds, MAX_FDS, TIME_OUT);
    if (nready == 0)
      continue;
    else if (nready == -1) {
      perror("\npoll(): pollfds");
      continue;
    }

    for (int i = 4; i < MAX_FDS; i++) {
      //@@
      printf("@check: %d\n", i);

      if (pollfds[i].fd == -1 || (pollfds[i].revents & POLLIN) == 0)
        continue;

      connfd = i;
      while (1) {
        retval = recv_msg(connfd, client_msg, sessions[i].recv_buffer);
        if (retval > 0) {
          handle_msg(client_msg, &sessions[i]);
          continue;
        } else {
          if (retval == -3) {
            // fprintf(stderr, "Message too long\n", LEN_MSG);
            send_msg(connfd, MSG_NOT_DETERMINED);
            break;
          } else if (retval == -2) {
            // Connection closed
            close(connfd);
            printf("[%d:%d]: (Disconnected)\n", connfd,
                   sessions[connfd].user_id);
            sessions[connfd].connfd = 0;
            count_sessions--;
            pollfds[connfd].fd = -1;
            break;
          } else if (retval == -1) {
            // recv() error
            fprintf(stderr, "Can not get message from \"%s\"'s client\n",
                    get_username(sessions[connfd].user_id));
            break;
          } else if (retval == 0) {
            // recv() success but can not get message
          }
        }

        if (strstr(sessions[i].recv_buffer, DELIMITER) != NULL)
          continue;

        if (poll(&pollfds[connfd], 1, 0) > 0)
          if ((pollfds[i].revents & POLLIN) != 0)
            continue;

        break;
      }

      nready--;
      if (nready == 0)
        break;
    }
  }
  fprintf(stderr, "\nServer has error!!");
  close(serverfd);
  return 0;
}