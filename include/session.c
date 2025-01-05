#include "session.h"

session_t sessions[MAX_SESSIONS] = {0};
int count_sessions = 0;

// int create_session(int connfd) {
//   for (int i = 0; i < MAX_SESSIONS; i++) {
//     if (sessions[i].connfd == 0) {
//       sessions[count_sessions].connfd = connfd;
//       count_sessions++;
//       return 0;
//     }
//   }
//   fprintf(stderr, "create_session(): Cannot create new session\n");
//   return -1;
// }

// int delete_session(int connfd) {
//   for (int i = 0; i < MAX_SESSIONS; i++) {
//     if (sessions[i].connfd == connfd) {
//       memset(&sessions[i], 0, sizeof(session_t));
//       count_sessions--;
//       return 0;
//     }
//   }
//   fprintf(stderr, "delete_session(): Session %d not found\n", connfd);
//   return -1;
// }
