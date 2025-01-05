#include "test_lib.h"

int __test = 555;
char __src[4] = "abc";

int assign(char **p) {
  *p = __src;
  // *p = "src";
  return 0;
}