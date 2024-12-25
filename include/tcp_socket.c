#include "tcp_socket.h"

char G_msg[MSG_SIZE + 10] = {0};   // received message
char G_s_msg[MSG_SIZE + 10] = {0}; // message will be send
char G_buffer[MSG_SIZE*10] = {0};  // buffer for received message

int send__(int fd, void *buf, size_t n, int flags) {
  int sent_size = send(fd, buf, n, flags);
  if (sent_size < 0) {
    perror("Error sending message");
    return -1;
  } else if (sent_size < n) {
    perror("Can not send all bytes");
    return -2;
  }

#ifdef DEBUG_SEND_RECV
  printf("%2d -> %s~\n", fd, (char *)buf);
#endif
  return sent_size;
}

int recv__(int fd, void *buf, size_t n, int flags) {
  int read_size = recv(fd, buf, n, flags);
  if (read_size < 0) {
    perror("Error receiving message");
    return -1;
  }
  char *p = (char *)buf;
  p[read_size] = '\0';

#ifdef DEBUG_SEND_RECV_2
  printf("%2d -> %s~\n", fd, (char *) buf);
#endif
  return read_size;
}
//________________________________________________________________________________________
/**
 * @brief Send a message to a socket
 */
int send_msg(int fd, char *__msg) {
  if (strlen(__msg) > MSG_SIZE) {
    perror("Message too long");
    return -1;
  }
  strcpy(G_s_msg, __msg);
  strcat(G_s_msg, DELIMITER);

  if (send__(fd, G_s_msg, strlen(G_s_msg), 0) == strlen(G_s_msg)) {
#ifdef DEBUG_SEND_RECV_2
    printf(" -> msg:%s~\n", (char *)__msg);
#endif
    return 0;
  } else {
    return -1;
  }
}

/**
 * @brief Receive a message from a socket
 */
int recv_msg(int fd, char *buf, char *msg_buf) {
  int ready_flag = 0;
  if (getmsg(G_msg, msg_buf) != 0)
    ready_flag = 1;

  if (ready_flag == 0) {
    do {
      recv__(fd, G_buffer, sizeof(G_buffer) - 1, 0);
    } while (getmsg_2(G_msg, G_buffer, msg_buf) == 0);
    ready_flag = 1;
  }
  strcpy(buf, G_msg);

#ifdef DEBUG_SEND_RECV
  printf(" <- msg:%s~\n", G_msg);
#endif

  return strlen(G_msg);
}
//________________________________________________________________________________________
int getmsg(char *buf, char *msg_buf) {
  char *p = strstr(msg_buf, DELIMITER);
  if (p != NULL) {
    strncpy(buf, msg_buf, p - msg_buf);
    int msg_size = p - msg_buf;
    buf[msg_size] = '\0';

    strcpy(msg_buf, p + DELIMITER_LEN);
    return msg_size;
  }
  return 0;
}

int getmsg_2(char *buf, char *recv_buf, char *msg_buf) {
  char *p = strstr(recv_buf, DELIMITER);
  if (p != NULL) {
    if (msg_buf[0] == '\0') {
      strncpy(buf, recv_buf, p - recv_buf);
      int msg_size = p - recv_buf;
      buf[msg_size] = '\0';

      strcpy(msg_buf, p + DELIMITER_LEN);
      return msg_size;
    } else {
      strcpy(buf, msg_buf);
      strncat(buf, recv_buf, p - recv_buf);
      int msg_size = strlen(msg_buf) + (p - recv_buf);
      buf[msg_size] = '\0';

      strcpy(msg_buf, p + DELIMITER_LEN);
      return msg_size;
    }
  }
  return 0;
}