#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char __time[30];
char inp[LEN_MSG + 1] = {0};

int add_element2(int (*arr)[2], int size, int value1, int value2) {
  for (int i = 0; i < size; i++) {
    if (arr[i][0] == value1 && arr[i][1] == value2)
      return 1;
    if (arr[i][0] == 0) {
      arr[i][0] = value1;
      arr[i][1] = value2;
      return 0;
    }
  }
  return 2;
}

bool remove_element2(int (*arr)[2], int size, int value1, int value2) {
  int idx1 = -1;
  for (int i = 0; i < size; i++) {
    if (arr[i][0] == value1 && arr[i][1] == value2) {
      idx1 = i;
      break;
    }
  }
  if (idx1 == -1)
    return false;
  
  int idx2 = -1;
  for (int i = idx1; i < size; i++) {
    if (arr[i][0] == 0) {
      idx2 = i - 1;
      break;
    }
  }
  if (idx2 == -1)
    idx2 = size - 1;
  
  arr[idx1][0] = arr[idx2][0];
  arr[idx1][1] = arr[idx2][1];
  arr[idx2][0] = 0;
  arr[idx2][1] = 0;
  return true;
}

int remove_element2e2(int (*arr)[2], int size, int value2) {
  int count = 0;
  int idx1 = -1;
  for (int i = 0; i < size; i++) {
    if (arr[i][1] == value2) {
      return 0;
    }
  }

  int idx2 = -1;
  for (int i = 0; i < size; i++) {
    if (arr[i][0] == 0) {
      idx2 = i - 1;
      break;
    }
  }
  if (idx2 == -1)
    idx2 = size - 1;
  
  arr[idx1][0] = arr[idx2][0];
  arr[idx1][1] = arr[idx2][1];
  arr[idx2][0] = 0;
  arr[idx2][1] = 0;
  count++;

  idx1++;
  idx2--;
  while (idx1 <= idx2) {
    if (arr[idx1][1] == value2) {
      arr[idx1][0] = arr[idx2][0];
      arr[idx1][1] = arr[idx2][1];
      arr[idx2][0] = 0;
      arr[idx2][1] = 0;
      count++;
      idx2--;
    }
    idx1++;
  }
  return count;
}

/**
 * Add element to array
 * Return 0 if success
 *        1 if value is already in array
 *        2 if array is full
 */
int add_element(int *arr, int size, int value) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == value)
      return 1;
    if (arr[i] == 0) {
      arr[i] = value;
      return 0;
    }
  }
  return 2;
}

bool remove_element(int *arr, int size, int value) {
  int idx1 = -1;
  for (int i = 0; i < size; i++) {
    if (arr[i] == value) {
      idx1 = i;
      break;
    }
  }
  if (idx1 == -1)
    return false;
  
  int idx2 = -1;
  for (int i = idx1; i < size; i++) {
    if (arr[i] == 0) {
      idx2 = i - 1;
      break;
    }
  }
  if (idx2 == -1)
    idx2 = size - 1;
  
  arr[idx1] = arr[idx2];
  arr[idx2] = 0;
  return true;
}

bool existed(int *arr, int size, int value) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == value)
      return true;
  }
  return false;
}

char *fgets_(char **dest) {
  if (inp == NULL)
    return NULL;
  
  char *retval = fgets(inp, sizeof(inp), stdin);
  if (retval == NULL) {
    fprintf(stderr, "fgets(): failed\n");
    return NULL;
  }

  if (*inp == '\n')
    return NULL;
  
  if (inp[strlen(inp) - 1] != '\n') {
    fprintf(stderr, "fgets_(): input too long\n");
    clear_stdin();
  }

  inp[strlen(inp) - 1] = '\0';
  *dest = trim_blank(inp);
  return retval;
}

/**
 * read integer from string less than 9 digits
 * Return 0 if success
 *        1 if cannot read integer
 */
int strtol_(char **src_p, int *dest) {
  char *src = *src_p;
  int num;
  if (!isdigit(next_char(src)))
    return 1;
  char *p;
  num = strtol(src, &p, 10);
  if (p - src > 9)
    return 1;
  if (*p != ' ' && *p != '\0')
    return 1;

  *src_p = p;
  *dest = num;
  return 0;
}

/**
 * !! max length of dest must be less than LEN_MSG
 * Return 0 if success
 *       1 if src is empty
 *       2 if string too long
 */
int sscanf_(char **src_p, char *dest) {
  char *src = *src_p;
  char buf[LEN_MSG + 1];
  char format[10];
  sprintf(format, "%%%ds", LEN_MSG);
  int retval = sscanf(src, format, buf);
  assert(retval != 0);
  if (retval == -1)
    return 1;
  
  if (strlen(buf) > (sizeof(dest) - 1) || strlen(buf) > LEN_MSG)
    return 2;


  strcpy(dest, buf);
  char *p = strstr(src, buf);
  *src_p = p + strlen(buf);
  return 0;
}

/**
 * !! max_len must be less than max length of dest and LEN_MSG
 * Return 0 if success
 *        1 if src is empty
 *        2 if string too long
 */
int sscanf_2(char **src_p, char *dest, int max_len) {
  char *src = *src_p;
  char buf[LEN_MSG + 1];
  char format[10];
  sprintf(format, "%%%ds", LEN_MSG);
  int retval = sscanf(src, format, buf);
  assert(retval != 0);
  if (retval == -1)
    return 1;
  
  if (strlen(buf) > max_len || strlen(buf) > LEN_MSG)
    return 2;

  strcpy(dest, buf);
  char *p = strstr(src, buf);
  *src_p = p + strlen(buf);
  return 0;
}

char next_char(char *str) {
  if (str == NULL)
    return '\0';
  while (*str == ' ')
    str++;
  
  return *str;
}

bool is_blank(char *str) {
  if (str == NULL)
    return false;

  while (*str != '\0') {
    if (*str != ' ')
      return false;
    str++;
  }
  return true;
}

char *skip_blank(char *str) {
  while (*str == ' ')
    str++;
  return str;
}

char *skip_2blank(char *str) {
  if (str == NULL)
    return NULL;
  while (*str == ' ')
    str++;
  while (*str != ' ' && *str != '\0')
    str++;
  while (*str == ' ')
    str++;
  return str;
}

char *trim_blank(char *str) {
  if (str == NULL)
    return NULL;
  while (*str == ' ')
    str++;
  if (*str == '\0')
    return str;

  char *end = str + strlen(str) - 1;
  while (end > str && *end == ' ')
    end--;
  *(end + 1) = '\0';
  return str;
}

//______________________________________________________________________
// Clear stdin buffer
void clear_stdin() {
  char c = '\0';
  do {
    c = getchar();
  } while (c != EOF && c != '\n');
  return;
}

// get current time. Support for make log_file
char *get_time() {
  time_t current_time;
  struct tm *time_st;
  time(&current_time);
  time_st = localtime(&current_time);
  strftime(__time, sizeof(__time), "[%d/%m/%Y %H:%M:%S]", time_st);
  return __time;
}

// Get last character of a string
char get_last_char(char *str) {
  if (str == NULL || *str == '\0')
    return '\0';
  else
    return str[strlen(str) - 1];
}

int last_char(char *str, char c) {
  if (str == NULL || *str == '\0')
    return -1;
  
  str[strlen(str) - 1] = c;
  return 0;
}

char *last_of(char *str) {
  return &str[strlen(str)];
}


// Check if all characters in a string are alphanumeric
bool isalnum_(char *str) {
  char *p = str;
  while (*p != '\0') {
    if (!isalnum(*p))
      return false;
    p++;
  }
  return true;
}

bool is_available_port(int port) {
  if (port >= 1024 && port <= 49151)
    return true;
  return false;
}