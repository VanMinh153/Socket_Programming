#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* skip_space(char *str) {
  while (*str == ' ')
    str++;
  return str;
}

char* skip_2space(char *str) {
  while (*str == ' ')
    str++;
  while (*str != ' ' && *str != '\0')
    str++;
  while (*str == ' ')
    str++;
  return str;
}

char* trim_space(char *str) {
  char *end;
  while (*str == ' ')
    str++;
  if (*str == '\0')
    return str;
  
  end = str + strlen(str) - 1;
  while (end > str && *end == ' ')
    end--;
  *(end + 1) = '\0';
  return str;
}