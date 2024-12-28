#include "include_all.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  db_read_accounts();
  printf("Loaded %d accounts from %s\n", count_accounts, DB_ACCOUNTS);
  db_save_accounts();
  printf("Saved %d accounts to %s\n", count_accounts, DB_ACCOUNTS);
  return 0;
}