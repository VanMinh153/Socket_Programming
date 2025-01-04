// #include "include_all.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char str[] = "   abc   def   ";
  char *ptr = str;
  char str2[4] = {0};
  printf("~%s~\n", ptr);

  // ptr = trim_space(ptr);
  // printf("~%s~\n", ptr);

  // sscanf(ptr, "%s", str2);
  // printf("~%s~\n", str2);

  // strncpy(str2, ptr, 13);
  // printf("~%s~\n", str2);

  strcpy(str2, ptr);
  printf("~%s~\n", str2);
  
  return 0;
}
