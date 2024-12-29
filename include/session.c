#include "session.h"

session_t sessions[MAX_SESSIONS] = {0};
int count_sessions = 0;

int create_session(int connfd) {
  if (count_sessions >= MAX_SESSIONS) {
    return -1;
  }
  for (int i = 0; i < count_sessions; i++) {
    if (sessions[i].connfd == 0) {
      sessions[count_sessions].connfd = connfd;
      count_sessions++;
      return 0;
    }
  }
  return 0;
}

int delete_session(int connfd) {
  for (int i = 0; i < count_sessions; i++) {
    if (sessions[i].connfd == connfd) {
      memset(&sessions[i], 0, sizeof(session_t));
      count_sessions--;
      return 0;
    }
  }
  return -1;
}