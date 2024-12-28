#include "utils.h"

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