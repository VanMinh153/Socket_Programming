#include "include_all.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST event

int main() {
  printf("!! Test program !!\n");
  char test[] = "event";
  //______________________________________________________________________
  while (strstr(test, "event") != NULL) {
    printf("# Test event.h\n");
    printf("Testing: db_read_events()\n");
    db_read_events();
    printf("Loaded %d events from %s\n", count_events, DB_EVENTS);

    printf("Testing: db_save_events()\n");
    db_save_events();
    printf("Saved %d events to %s\n", count_events, DB_EVENTS);
    break;
  }
  //______________________________________________________________________
  while (strstr(test, "account") != NULL) {
    printf("# Test account.h\n");
    printf("Testing: db_read_accounts()\n");
    db_read_accounts();
    printf("Loaded %d accounts from %s\n", count_accounts, DB_ACCOUNTS);
    break;

    printf("Testing: db_save_accounts()\n");
    db_save_accounts();
    printf("Saved %d accounts to %s\n", count_accounts, DB_ACCOUNTS);
  }

  //______________________________________________________________________
  while (strstr(test, "utils") != NULL) {
    printf("# Test utils.h\n");
    char str[] = "   abc   def   ";
    char *ptr = str;
    printf("~%s~\n", ptr);
    break;

    printf("Testing: skip_blank()\n");
    ptr = skip_blank(str);
    printf("~%s~\n", ptr);

    printf("Testing: skip_2blank()\n");
    ptr = skip_2blank(str);
    printf("~%s~\n", ptr);

    printf("Testing: trim_blank()\n");
    ptr = trim_blank(str);
    printf("~%s~\n", ptr);
  }
  return 0;
}