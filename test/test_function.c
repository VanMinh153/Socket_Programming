#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

char* skip_space(char *str) {
  while (*str == ' ')
    str++;
  return str;
}

char* skip_2space(char *str) {
  while (*str == ' ')
    str++;
  while (*str != ' ')
    str++;
  while (*str == ' ')
    str++;
  return str;
}

int main() {
  // char str[] = "   123   456";
  char str[] = "   abc   def   ";
  char *ptr = str;
  char str2[21] = {0};

  printf("~%s~\n", ptr);
  ptr = skip_space(str);
  printf("~%s~\n", ptr);

  ptr = skip_2space(str);
  printf("~%s~\n", ptr);
  
  return 0;
}
