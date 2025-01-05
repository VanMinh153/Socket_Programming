#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "utils.h"

int test(session_t* _session) {
  session_t session = *_session;
  session.signed_in = true;
}

int main() {


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
