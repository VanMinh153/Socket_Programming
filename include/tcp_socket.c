#include "tcp_socket.h"

char __msg[MSG_BUFFER_SIZE] = {0};      // received message
char __send_msg[MSG_BUFFER_SIZE] = {0}; // message will be send
char __buffer[MSG_BUFFER_SIZE] = {0};   // buffer for received message

bool connection_is_active() {
  switch (errno) {
  case ECONNRESET:   // Connection reset by peer
  case ESHUTDOWN:    // Cannot send after socket shutdown
  case ENOTCONN:     // Socket is not connected
  case ECONNREFUSED: // Connection refused by server
  case ECONNABORTED: // Connection aborted
  case EPIPE:        // Broken pipe
  case EHOSTDOWN:    // Host is down
  case EHOSTUNREACH: // Host is unreachable
    fprintf(stderr, "Connection error\n");
    return false;
  default:
    return true;
  }
}

/**
 * @brief Send a message to a socket
 * @return
 *     n if success
 *    -1 have error
 *    -2 connection closed
 */
int send_(int fd, void *buf, size_t n, int flags) {
  int sent_size = 0;
  while (sent_size < n) {
    int retval = send(fd, buf + sent_size, n - sent_size, flags);
    if (retval == -1) {
      perror("send()");
      if (!connection_is_active())
        return -2;
      return -1;
    }
    sent_size += retval;
  }

#ifdef TEST_TCP_SOCKET_1
  printf(" %d -> `%s`\n", fd, (char *)buf);
#endif
  return n;
}

/**
 * @brief Send a message to a socket
 * @return
 *    (N > 0) success: N byte readed
 *    -1 have error
 *    -2 Connection closed
 */
int recv_(int fd, void *buf, size_t n, int flags) {
  int read_size = recv(fd, buf, n, flags);
  if (read_size == 0)
    return -2;
  if (read_size == -1) {
    perror("\nrecv()");
    if (!connection_is_active())
      return -2;
    return -1;
  }

  char *p = (char *)buf;
  p[read_size] = '\0';

#ifdef TEST_TCP_SOCKET_1
  printf(" %d <- `%s`\n", fd, (char *)buf);
#endif
  return read_size;
}
//______________________________________________________________________
/**
 * @brief Send a message to a socket
 * @return
 *     0 if success
 *    -1 have error
 *    -2 Can not send all bytes
 *    -3 Message overlength
 */
int send_msg(int fd, char *msg) {
  int msg_len = strlen(msg);
  if (msg_len > LEN_MSG) {
    fprintf(stderr, "send_msg(): Message too long\n");
    return -3;
  }
  strcpy(__send_msg, msg);
  strcat(__send_msg, DELIMITER);
  msg_len += 2;

  int retval = send_(fd, __send_msg, msg_len, 0);
  if (retval == msg_len) {
#ifdef TEST_TCP_SOCKET_2
    printf("@%d -> `%s`\n", fd, msg);
#endif
    return 0;
  } else {
    return retval;
  }
}

/**
 * @brief Receive a message from a socket
 * @return
 *     2 if success to get a message from msg_buf
 *     1 if success to call recv_() and get a message
 *     0 no message had received
 *    -1 have error
 *    -2 Connection closed
 *    -3 Message overlength
 */
int recv_msg(int fd, char *msg, char *msg_buf) {
  int ready_flag = 0;
  int retval = getmsg(msg, msg_buf);
  if (retval > 0) {
#ifdef TEST_TCP_SOCKET_2
    printf("@%d <- `%s`\n", fd, msg);
#endif
    return 2;
  } else if (retval == -1)
    return -3;

  retval = recv_(fd, __buffer, sizeof(__buffer) - 1, 0);
  if (retval < 0)
    return retval;

  retval = getmsg_2(msg, __buffer, msg_buf);
  if (retval > 0) {
#ifdef TEST_TCP_SOCKET_2
    printf("@%d <- `%s`\n", fd, msg);
#endif
    return 1;
  } else if (retval == -1)
    return -3;
  
  return 0;
}
//______________________________________________________________________
int getmsg(char *msg, char *msg_buf) {
  char *p = strstr(msg_buf, DELIMITER);
  if (p == NULL)
    return 0;

  int msg_size = p - msg_buf;
  if (msg_size > LEN_MSG) {
    fprintf(stderr, "getmsg(): Message too long\n");
    return -1;
  }
  memcpy(msg, msg_buf, p - msg_buf);
  msg[msg_size] = '\0';

  strcpy(msg_buf, p + LEN_DELIMITER);
  return msg_size;
}

int getmsg_2(char *msg, char *recv_buf, char *msg_buf) {
  char *p = strstr(recv_buf, DELIMITER);
  if (p == NULL)
    return 0;

  if (msg_buf[0] == '\0') {
    int msg_size = p - recv_buf;
    if (msg_size > LEN_MSG) {
      fprintf(stderr, "getmsg_2(): Message too long\n");
      return -1;
    }
    memcpy(msg, recv_buf, p - recv_buf);
    msg[msg_size] = '\0';
    strcpy(msg_buf, p + LEN_DELIMITER);
    return msg_size;
  } else {
    int msg_size = strlen(msg_buf) + (p - recv_buf);
    if (msg_size > LEN_MSG) {
      fprintf(stderr, "getmsg_2(): Message too long\n");
      return -1;
    }
    strcpy(msg, msg_buf);
    strncat(msg, recv_buf, p - recv_buf);
    msg[msg_size] = '\0';
    strcpy(msg_buf, p + LEN_DELIMITER);
    return msg_size;
  }
}