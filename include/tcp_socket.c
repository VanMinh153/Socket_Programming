
#include "tcp_socket.h"

char __msg[MSG_BUFFER_SIZE] = {0};      // received message
char __send_msg[MSG_BUFFER_SIZE] = {0}; // message will be send
char __buffer[MSG_BUFFER_SIZE] = {0};   // buffer for received message

bool connection_is_active() {
  switch (errno) {
    case ECONNRESET:    // Connection reset by peer
    case ESHUTDOWN:     // Cannot send after socket shutdown
    case ENOTCONN:      // Socket is not connected
    case ECONNREFUSED:  // Connection refused by server
    case ECONNABORTED:  // Connection aborted
    case EPIPE:         // Broken pipe
    case EHOSTDOWN:     // Host is down
    case EHOSTUNREACH:  // Host is unreachable
      fprintf(stderr, "Connection error\n");
      return false;
    default:
      return true;
  }
}



/**
 * @brief Send a message to a socket
 * @return
 *     0 if success
 *    -1 have error
 *    -2 connection closed
 *    -3 Can not send all bytes
 */
int _send(int fd, void *buf, size_t n, int flags) {
  int sent_size = send(fd, buf, n, flags);
  if (sent_size == -1) {
    perror("send()");
    if (!connection_is_active())
      return -2;
  } else if (sent_size < n) {
    perror("_send(): Can not send all bytes");
    return -2;
  }

#ifdef TEST_TCP_SOCKET_1
  printf("%2d -> %s~\n", fd, (char *)buf);
#endif
  return sent_size;
}

/**
 * @brief Send a message to a socket
 * @return
 *    (N > 0) success: N byte readed
 *    -1 have error
 *    -2 Connection closed
 */
int _recv(int fd, void *buf, size_t n, int flags) {
  int read_size = recv(fd, buf, n, flags);
  if (read_size <= 0) {
    if (read_size == 0 || errno == ECONNRESET)
      return -2;
    else {
      perror("\nrecv()");
      return -1;
    }
  }
  char *p = (char *)buf;
  p[read_size] = '\0';

#ifdef TEST_TCP_SOCKET_1
  printf("%2d -> %s~\n", fd, (char *)buf);
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
  int retval;
  if (strlen(msg) > LEN_MSG) {
    perror("Message too long");
    return -3;
  }
  strcpy(__send_msg, msg);
  strcat(__send_msg, DELIMITER);

  retval = _send(fd, __send_msg, strlen(__send_msg), 0);
  if (retval == strlen(__send_msg)) {
#ifdef DEBUG_SEND_RECV_2
    printf(" -> msg:%s~\n", (char *)msg);
#endif
    return 0;
  } else {
    return retval;
  }
}

/**
 * @brief Receive a message from a socket
 * @return
 *     0 if success
 *    -1 have error
 *    -2 Connection closed
 *    -3 Message overlength
 */
int recv_msg(int fd, char *msg, char *msg_buf) {
  int retval;
  int ready_flag = 0;
  if (getmsg(__msg, msg_buf) != 0)
    ready_flag = 1;

  if (ready_flag == 0) {
    retval = _recv(fd, __buffer, sizeof(__buffer) - 1, 0);
    if (retval < 0) {
      return retval;
    }
    if (getmsg_2(__msg, __buffer, msg_buf) == 0)

      ready_flag = 1;
  }
  strcpy(msg, __msg);

#ifdef DEBUG_SEND_RECV
  printf(" <- msg:%s~\n", __msg);
#endif
  return strlen(__msg);
}
//______________________________________________________________________
int getmsg(char *msg, char *msg_buf) {
  char *p = strstr(msg_buf, DELIMITER);
  if (p != NULL) {
    memcpy(msg, msg_buf, p - msg_buf);
    int msg_size = p - msg_buf;
    msg[msg_size] = '\0';

    strcpy(msg_buf, p + LEN_DELIMITER);
    return msg_size;
  }
  return 0;
}

int getmsg_2(char *msg, char *recv_buf, char *msg_buf) {
  char *p = strstr(recv_buf, DELIMITER);
  if (p != NULL) {
    if (msg_buf[0] == '\0') {
      memcpy(msg, recv_buf, p - recv_buf);
      int msg_size = p - recv_buf;
      msg[msg_size] = '\0';

      strcpy(msg_buf, p + LEN_DELIMITER);
      return msg_size;
    } else {
      strcpy(msg, msg_buf);
      strncat(msg, recv_buf, p - recv_buf);
      int msg_size = strlen(msg_buf) + (p - recv_buf);
      msg[msg_size] = '\0';

      strcpy(msg_buf, p + LEN_DELIMITER);
      return msg_size;
    }
  }
  return 0;
}