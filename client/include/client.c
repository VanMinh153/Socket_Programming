#include "client.h"



int print_msg(char *msg) {
  if (*msg == '\0') {
    fprintf(stderr, "print_msg(): message is empty\n");
    return -1;
  }
  printf("%s\n", msg);
  return 0;
}