#include "account.h"
#include "session.h"
#include <regex.h>

// Global variables
account_t accounts[MAX_ACCOUNTS] = {0};
int count_accounts = 0;
int gen_account_id = 1;

/**
 * Return 0 if account is created successfully
 *        1 if account database is full
 *        2 if username format is incorrect
 *        3 if password format is incorrect
 *        4 if username already exists
 */
int create_account(char *username, char *password) {
  if (count_accounts >= MAX_ACCOUNTS) {
    fprintf(stderr, "!! Account database is full !!\n");
    return 1;
  }

  if (check_format_username(username) == 1)
    return 2;
  if (check_format_password(password) == 1)
    return 3;

  // Check if username already exists
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0)
      return 4;
  }

  // Create new account
  accounts[count_accounts].id = gen_account_id;
  memcpy(accounts[count_accounts].username, username, strlen(username) + 1);
  memcpy(accounts[count_accounts].password, password, strlen(password) + 1);
  gen_account_id++;
  count_accounts++;
  return 0;
}

int get_user_idx(int user_id) {
  for (int i = 0; i < count_accounts; i++) {
    if (accounts[i].id == user_id)
      return i;
  }
  fprintf(stderr, "User ID not found: %d\n", user_id);
  return -1;
}

int get_user_id(char *username) {
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0)
      return accounts[i].id;
  }
  fprintf(stderr, "Username not found: \"%s\"\n", username);
  return -1;
}

char *get_username(int user_id) {
  int idx = get_user_idx(user_id);
  if (idx == -1)
    return NULL;
  return accounts[idx].username;
}

bool user_exists(int user_id) {
  return get_user_idx(user_id) != -1;
}

/**
 * Return 0 if password is correct
 *        1 if user is not found
 *        2 if password is incorrect
 */
int check_password(int user_id, char *password) {
  int idx = get_user_idx(user_id);
  if (idx == -1)
    return 1;
  
  if (strcmp(accounts[idx].password, password) == 0)
    return 0;
  return 2;
}

/**
 * Username format: [a-zA-Z0-9]+(-[a-zA-Z0-9]+)*
 * Return 0 if username format is correct
 *        1 if username format is incorrect
 */
int check_format_username(char *username) {
  if (strlen(username) > LEN_USERNAME) {
    return 1; // Username is too long
  }
  regex_t regex;
  int ret;

  // Compile regex pattern for username
  ret = regcomp(&regex, USERNAME_REGEX, REG_EXTENDED);
  if (ret) {
    perror("Error compiling regex");
    exit(1); // Regex compilation failed
  }

  // Execute regex match
  ret = regexec(&regex, username, 0, NULL, 0);
  regfree(&regex);

  if (ret == 0) {
    return 0; // Format is correct
  }
  return 1; // Format is incorrect
}

/**
 * Password format: [a-zA-Z0-9]+ (alphanumeric characters)
 * Return 0 if password format is correct
 *        1 if password format is incorrect
 */
int check_format_password(char *password) {
  if (strlen(password) > LEN_PASSWORD) {
    return 1; // Password is too long
  }  
  for (int i = 0; password[i] != '\0'; i++) {
    if (!isalnum(password[i])) {
      return 1;
    }
  }
  return 0;
}

int db_read_accounts() {
  FILE *fp = fopen(DB_ACCOUNTS, "r");
  if (fp == NULL) {
    perror("Error opening accounts database");
    exit(1);
  }

  char line[MAX_LINE_TEXT];
  count_accounts = 0;
  gen_account_id = 1;

  while (fgets(line, sizeof(line), fp)) {
    char *token;
    int i = 0;

    // Parse ID
    token = strtok(line, "|");
    if (token == NULL)
      continue;
    accounts[count_accounts].id = atoi(token);
    gen_account_id++;

    // Parse username
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_USERNAME) {
      fprintf(stderr, "Username too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    if (check_format_username(token) == 1) {
      fprintf(stderr, "Invalid username format: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(accounts[count_accounts].username, token, strlen(token) + 1);
    

    // Parse password
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_PASSWORD) {
      fprintf(stderr, "Password too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    if (check_format_password(token) == 1) {
      fprintf(stderr, "Invalid password format: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(accounts[count_accounts].password, token, strlen(token) + 1);

    // Parse friends
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_FRIENDS) {
      accounts[count_accounts].friends[i++] = atoi(token);
      token = skip_2blank(token);
    }

    // Parse friend requests
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_FRIEND_REQUESTS) {
      accounts[count_accounts].friend_requests[i++] = atoi(token);
      token = skip_2blank(token);
    }

    // Parse event joins
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_EVENT_JOINS) {
      accounts[count_accounts].event_joins[i++] = atoi(token);
      token = skip_2blank(token);
    }

    // Parse event requests
    token = strtok(NULL, "\n");
    if (token == NULL) {
      count_accounts++;
      continue;
    }
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_EVENT_REQUESTS) {
      sscanf(token, "%d,%d", &accounts[count_accounts].event_requests[i][0],
             &accounts[count_accounts].event_requests[i][1]);
      i++;
      token = skip_2blank(token);
    }
    count_accounts++;
  }

  fclose(fp);

#ifdef TEST_db_read_accounts
  printf("Read %d accounts\n", count_accounts);
  printf("ID | Username | Password |  Friends  | Friend Requests | Event Joins "
         "| Event Requests\n");

  for (int i = 0; i < count_accounts; i++) {
    int j;
    printf("%2d | %8s | %8s | ", accounts[i].id, accounts[i].username,
           accounts[i].password);

    for (j = 0; j < MAX_FRIENDS && accounts[i].friends[j] != 0; j++) {
      printf("%d ", accounts[i].friends[j]);
    }
    printf("%*s", abs(9 - 2 * j), "");
    printf(" | ");

    for (j = 0; j < MAX_FRIEND_REQUESTS && accounts[i].friend_requests[j] != 0; j++) {
      printf("%d ", accounts[i].friend_requests[j]);
    }
    printf("%*s", abs(15 - 2 * j), "");
    printf(" | ");

    for (j = 0; j < MAX_EVENT_JOINS && accounts[i].event_joins[j] != 0; j++) {
      printf("%d ", accounts[i].event_joins[j]);
    }
    printf("%*s", abs(11 - 2 * j), "");
    printf(" | ");

    for (j = 0; j < MAX_EVENT_REQUESTS && accounts[i].event_requests[j][0] != 0; j++) {
      printf("%d,%d ", accounts[i].event_requests[j][0],
             accounts[i].event_requests[j][1]);
    }
    printf("\n");
  }
#endif
  return 0;
}

int db_save_accounts0() {
  FILE *fp = fopen(DB_ACCOUNTS_BACKUP, "w");
  if (fp == NULL) {
    perror("Error opening accounts database");
    exit(1);
  }

  for (int i = 0; i < count_accounts; i++) {
    // Write ID, username, and password
    fprintf(fp, "%d | %s | %s | ", accounts[i].id, accounts[i].username,
            accounts[i].password);

    // Write friends
    for (int j = 0; j < MAX_FRIENDS && accounts[i].friends[j] != 0; j++) {
      fprintf(fp, "%d ", accounts[i].friends[j]);
    }
    fprintf(fp, "| ");

    // Write friend requests
    for (int j = 0;
         j < MAX_FRIEND_REQUESTS && accounts[i].friend_requests[j] != 0; j++) {
      fprintf(fp, "%d ", accounts[i].friend_requests[j]);
    }
    fprintf(fp, "| ");

    // Write event joins
    for (int j = 0; j < MAX_EVENT_JOINS && accounts[i].event_joins[j] != 0;
         j++) {
      fprintf(fp, "%d ", accounts[i].event_joins[j]);
    }
    fprintf(fp, "| ");

    // Write event requests
    for (int j = 0;
         j < MAX_EVENT_REQUESTS && accounts[i].event_requests[j][0] != 0; j++) {
      fprintf(fp, "%d,%d ", accounts[i].event_requests[j][0],
              accounts[i].event_requests[j][1]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}

int db_save_accounts() {
  FILE *fp = fopen(DB_ACCOUNTS_BACKUP, "w");
  if (fp == NULL) {
    perror("Error opening accounts database");
    exit(1);
  }

  for (int i = 0; i < count_accounts; i++) {
    int j;
    fprintf(fp, "%d | %8s | %8s | ", accounts[i].id, accounts[i].username,
           accounts[i].password);

    for (j = 0; j < MAX_FRIENDS && accounts[i].friends[j] != 0; j++) {
      fprintf(fp, "%d ", accounts[i].friends[j]);
    }
    fprintf(fp, "%*s", abs(9 - 2 * j), "");
    fprintf(fp, " | ");

    for (j = 0; j < MAX_FRIEND_REQUESTS && accounts[i].friend_requests[j] != 0; j++) {
      fprintf(fp, "%d ", accounts[i].friend_requests[j]);
    }
    fprintf(fp, "%*s", abs(15 - 2 * j), "");
    fprintf(fp, " | ");

    for (j = 0; j < MAX_EVENT_JOINS && accounts[i].event_joins[j] != 0; j++) {
      fprintf(fp, "%d ", accounts[i].event_joins[j]);
    }
    fprintf(fp, "%*s", abs(11 - 2 * j), "");
    fprintf(fp, " | ");

    for (j = 0; j < MAX_EVENT_REQUESTS && accounts[i].event_requests[j][0] != 0; j++) {
      fprintf(fp, "%d,%d ", accounts[i].event_requests[j][0],
             accounts[i].event_requests[j][1]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}
