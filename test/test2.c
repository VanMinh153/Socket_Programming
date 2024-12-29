#include "include_all.h"

int main() {
  char str[] = "   abc   def   ";
  char *ptr = str;
  char str2[11] = {0};
  printf("~%s~\n", ptr);

  // ptr = trim_space(ptr);
  // printf("~%s~\n", ptr);

  // sscanf(ptr, "%s", str2);
  // printf("~%s~\n", str2);

  // strncpy(str2, ptr, 13);
  // printf("~%s~\n", str2);

  strlcpy(str2, ptr, 20);
  printf("~%s~\n", str2);
  
  return 0;
}
