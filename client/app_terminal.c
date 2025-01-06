#include "include/client.h"

#define send_t(msg) send_msg(clientfd, msg)
#define recv_f(msg) recv_msg(clientfd, msg, recv_buffer)

int SERVER_PORT;
int clientfd;
char send_m[LEN_MSG + 1] = {0};
char recv_m[LEN_MSG + 1] = {0};
char recv_buffer[LEN_MSG + 1] = {0};

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Please write: ./client <Server_IP> <Port_Number>\n");
    return 1;
  }
  struct sockaddr_in server_addr;
  int retval = -1;

  // init client
  clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientfd == -1) {
    perror("\nsocket()");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
    perror("Invalid IP address");
    return 1;
  }

  if (connect(clientfd, (struct sockaddr *)&server_addr,
              sizeof(struct sockaddr)) < 0) {
    perror("\nconnect()");
    return 1;
  }

  // Communicate with server
  //______________________________________________________________________
  recv_f(recv_m);
  print_msg(recv_m);

  // char command[LEN_MSG + 1] = {0};
  char *command = NULL;
  char inp[LEN_MSG + 1] = {0};
  char *clear_inp = NULL;

  while (1) {
    while (1) {
      printf("\nEnter command: ");
      if (fgets_(&command) == NULL)
        continue;
      break;
    }
#ifdef TEST_APP_TERMINAL
    // printf("command: `%s`\n", command);
#endif
    // printf("");
    send_t(command);
    recv_f(recv_m);
    if (strcmp(recv_m, OK2) == 0) {
      recv_f(recv_m);
      print_msg(recv_m);
      continue;
    }
    print_msg(recv_m);
  }
  return 0;
}
