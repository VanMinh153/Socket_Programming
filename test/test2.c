#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "utils.h"

int main() {
  char *str = NULL;
  fgets_(&str);
  printf("`%s`\n", str);
  fgets_(&str);
  printf("`%s`\n", str);

  // char str[] = "   -4  abc   ";
  // char *parse = str;
  // int x = 0;
  
  // printf("`%s`\n", parse);
  // strtol_(&parse, &x);
  // printf("%d\n", x);
  // printf("`%s`\n\n", parse);

  // parse = str;
  // x = 0;
  // sscanf_d2(&parse, &x);
  // printf("%d\n", x);
  // printf("`%s`\n", parse);
  

  // int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // for (int i = 0; i < 9; i++) {
  //   printf("%d ", arr[i]);
  // }
  // printf("\n");
  // remove_element(arr, 10, 5);
  // for (int i = 0; i < 9; i++) {
  //   printf("%d ", arr[i]);
  // }
  // printf("\n");


  // char str[50] = "";
  // printf("`%s`\n", str);

  // sprintf(str, "%s", "abc");
  // printf("`%s`\n", str);

  // last_char(str, '@');
  // printf("`%s`\n", str);

  // sprintf(last_of(str), "%s", "def");
  // printf("`%s`\n", str);
  return 0;
}
