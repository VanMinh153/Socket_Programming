#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "config.h"

extern char __time[30];

int add_element2(int **arr, int size, int value1, int value2);
bool remove_element2(int **arr, int size, int value1, int value2);
// int remove_element2e1(int **arr, int size, int value1);
int remove_element2e2(int **arr, int size, int value2);
int add_element(int *arr, int size, int value);
bool remove_element(int *arr, int size, int value);
bool existed(int *arr, int size, int value);
char *fgets_(char **str);
int sscanf_d(char **src, int *dest);
int sscanf_(char **src, char *dest); // Tested
int sscanf_2(char **src, char *dest, int max_len); // Tested

char next_char(char *str); // Tested
bool is_blank(char *str); // Tested
char *skip_blank(char *str);  // Tested
char *skip_2blank(char *str); // Tested
char *trim_blank(char *str);  // Tested

char get_last_char(char *str); // Tested
int last_char(char *str, char c); // Tested
char *last_of(char *str); // Tested

void clear_stdin(); // Tested
char *get_time(); // Tested
bool isalnum_(char *str); // Tested
bool is_available_port(int port); // Tested

#endif