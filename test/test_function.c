#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int main() {
  // char str[] = "   123   456";
  char str[] = "   abc   def";
  int int_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  void *ptr = int_arr;
  // ~ char *ptr = (char*) int_arr;
  ptr = ptr + 3;
  for (int i = 0; i < 9; i++) {
    printf("%d ", *((int*) ptr + i));
  }  
  return 0;
}
