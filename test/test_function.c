#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  char str[] = "  12 ab  ";
  char str2[20] = "abcdefghij";
  char c = 'a';
  int d = 0;
  // int retval = sscanf(str, "%s", str2);
  // printf("return: %d\n", retval);
  // printf("`%s`\n", str2);

  // int retval = sscanf(str, "%c", &c);
  // printf("return: %d\n", retval);
  // printf("`%c`\n", c);

  int retval = sscanf(str, "%9u", &d);
  printf("return: %d\n", retval);
  printf("`%u`\n", d);

  return 0;
}
