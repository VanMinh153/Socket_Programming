#include "account.h"
#include "config.h"
#include "session.h"
#include <regex.h>

// Global variables
account_t accounts[MAX_ACCOUNTS] = {0};
int count_accounts = 0;
int gen_account_id = 1;

// Helper function to find account by username
int find_account_by_username(const char *username) {
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      return i;
    }
  }
  return -1;
}

int handle_login(char *username, char *password) {
  int acc_idx = find_account_by_username(username);

  if (acc_idx == -1) {
    return 2; // Username not found
  }

  if (strcmp(accounts[acc_idx].password, password) == 0) {
    return 0; // Login successful
  }

  return 1; // Incorrect password
}

int handle_signup(char *username, char *password) {
  if (count_accounts >= MAX_ACCOUNTS) {
    return 1; // Account limit reached
  }

  if (find_account_by_username(username) != -1) {
    return 1; // Username already exists
  }

  // Create new account
  account_t new_account;
  new_account.id = gen_account_id++;
  strncpy(new_account.username, username, LEN_USERNAME);
  strncpy(new_account.password, password, LEN_PASSWORD);

  // Initialize arrays
  memset(new_account.friends, 0, sizeof(new_account.friends));
  memset(new_account.friend_requests, 0, sizeof(new_account.friend_requests));
  memset(new_account.event_joins, 0, sizeof(new_account.event_joins));
  memset(new_account.event_requests, 0, sizeof(new_account.event_requests));

  accounts[count_accounts++] = new_account;
  return 0;
}

int handle_change_password(char *username, char *cur_password,
                           char *new_password) {
  int acc_idx = find_account_by_username(username);

  if (acc_idx != -1 && strcmp(accounts[acc_idx].password, cur_password) == 0) {
    strncpy(accounts[acc_idx].password, new_password, LEN_PASSWORD);
    return 0;
  }

  return 1; // Current password incorrect
}

int check_format_username(char *username) {
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

int check_format_password(char *password) {
  // Check if password contains only alphanumeric characters
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
    sscanf(token, "%s", accounts[count_accounts].username);
    if (check_format_username(accounts[count_accounts].username) == 1) {
      perror("Invalid username format");
      fclose(fp);
      exit(1);
    }

    // Parse password
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    sscanf(token, "%s", accounts[count_accounts].password);
    if (check_format_password(accounts[count_accounts].password) == 1) {
      perror("Invalid password format");
      fclose(fp);
      exit(1);
    }

    // Parse friends
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_space(token);
    while (*token != '\0' && i < MAX_FRIENDS) {
      accounts[count_accounts].friends[i++] = atoi(token);
      token = skip_2space(token);
    }

    // Parse friend requests
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_space(token);
    while (*token != '\0' && i < MAX_FRIEND_REQUESTS) {
      accounts[count_accounts].friend_requests[i++] = atoi(token);
      token = skip_2space(token);
    }

    // Parse event joins
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_space(token);
    while (*token != '\0' && i < MAX_EVENT_JOINS) {
      accounts[count_accounts].event_joins[i++] = atoi(token);
      token = skip_2space(token);
    }

    // Parse event requests
    token = strtok(NULL, "\n");
    if (token == NULL) {
      count_accounts++;
      continue;
    }
    i = 0;
    token = skip_space(token);
    while (*token != '\0' && i < MAX_EVENT_REQUESTS) {
      sscanf(token, "%d,%d", &accounts[count_accounts].event_requests[i][0],
             &accounts[count_accounts].event_requests[i][1]);
      i++;
      token = skip_2space(token);
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
    printf("%d  | %8s | %8s | ", accounts[i].id, accounts[i].username,
           accounts[i].password);
    for (j = 0; j < MAX_FRIENDS; j++) {
      if (accounts[i].friends[j] == 0)
        break;
      printf("%d ", accounts[i].friends[j]);
    }
    printf("%*s", abs(9 - 2 * j) + 1, "");
    printf("| ");

    for (j = 0; j < MAX_FRIEND_REQUESTS; j++) {
      if (accounts[i].friend_requests[j] == 0)
        break;
      printf("%d ", accounts[i].friend_requests[j]);
    }
    printf("%*s", abs(15 - 2 * j) + 1, "");
    printf("| ");

    for (j = 0; j < MAX_EVENT_JOINS; j++) {
      if (accounts[i].event_joins[j] == 0)
        break;
      printf("%d ", accounts[i].event_joins[j]);
    }
    printf("%*s", abs(11 - 2 * j) + 1, "");
    printf("| ");

    for (j = 0; j < MAX_EVENT_REQUESTS; j++) {
      if (accounts[i].event_requests[j][0] == 0)
        break;
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
    fprintf(fp, "%d  | %8s | %8s | ", accounts[i].id, accounts[i].username,
           accounts[i].password);
    for (j = 0; j < MAX_FRIENDS; j++) {
      if (accounts[i].friends[j] == 0)
        break;
      fprintf(fp, "%d ", accounts[i].friends[j]);
    }
    fprintf(fp, "%*s", abs(9 - 2 * j) + 1, "");
    fprintf(fp, "| ");

    for (j = 0; j < MAX_FRIEND_REQUESTS; j++) {
      if (accounts[i].friend_requests[j] == 0)
        break;
      fprintf(fp, "%d ", accounts[i].friend_requests[j]);
    }
    fprintf(fp, "%*s", abs(15 - 2 * j) + 1, "");
    fprintf(fp, "| ");

    for (j = 0; j < MAX_EVENT_JOINS; j++) {
      if (accounts[i].event_joins[j] == 0)
        break;
      fprintf(fp, "%d ", accounts[i].event_joins[j]);
    }
    fprintf(fp, "%*s", abs(11 - 2 * j) + 1, "");
    fprintf(fp, "| ");

    for (j = 0; j < MAX_EVENT_REQUESTS; j++) {
      if (accounts[i].event_requests[j][0] == 0)
        break;
      fprintf(fp, "%d,%d ", accounts[i].event_requests[j][0],
             accounts[i].event_requests[j][1]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}

int create_friend_invites(char *username) {
  // Find the target account
  int target_idx = -1;
  int cur_idx = -1;

  // Find current user and target user indices
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      target_idx = i;
    }
  }

  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].logged) {
      for (int j = 0; j < count_accounts; j++) {
        if (strcmp(accounts[j].username, sessions[i].username) == 0) {
          cur_idx = j;
          break;
        }
      }
      break;
    }
  }

  if (target_idx == -1 || cur_idx == -1)
    return 1;

  // Check if already friends
  for (int i = 0; i < MAX_FRIENDS; i++) {
    if (accounts[cur_idx].friends[i] == accounts[target_idx].id) {
      return 2;
    }
  }

  // Add request to target's friend requests
  for (int i = 0; i < MAX_FRIEND_REQUESTS; i++) {
    if (accounts[target_idx].friend_requests[i] == 0) {
      accounts[target_idx].friend_requests[i] = accounts[cur_idx].id;
      return 0;
    }
  }

  return 3; // No space for new request
}

int take_back_friend_invites(char *username) {
  int target_idx = -1;
  int cur_idx = -1;

  // Find indices
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      target_idx = i;
    }
  }

  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].logged) {
      for (int j = 0; j < count_accounts; j++) {
        if (strcmp(accounts[j].username, sessions[i].username) == 0) {
          cur_idx = j;
          break;
        }
      }
      break;
    }
  }

  if (target_idx == -1 || cur_idx == -1)
    return 1;

  // Remove request from target's friend requests
  for (int i = 0; i < MAX_FRIEND_REQUESTS; i++) {
    if (accounts[target_idx].friend_requests[i] == accounts[cur_idx].id) {
      // Shift remaining requests
      for (int j = i; j < MAX_FRIEND_REQUESTS - 1; j++) {
        accounts[target_idx].friend_requests[j] =
            accounts[target_idx].friend_requests[j + 1];
      }
      accounts[target_idx].friend_requests[MAX_FRIEND_REQUESTS - 1] = 0;
      return 0;
    }
  }

  return 2; // Request not found
}

int accept_friend_request(char *username) {
  int requester_idx = -1;
  int cur_idx = -1;

  // Find indices
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      requester_idx = i;
    }
  }

  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].logged) {
      for (int j = 0; j < count_accounts; j++) {
        if (strcmp(accounts[j].username, sessions[i].username) == 0) {
          cur_idx = j;
          break;
        }
      }
      break;
    }
  }

  if (requester_idx == -1 || cur_idx == -1)
    return 1;

  // Verify request exists
  int request_found = 0;
  for (int i = 0; i < MAX_FRIEND_REQUESTS; i++) {
    if (accounts[cur_idx].friend_requests[i] == accounts[requester_idx].id) {
      request_found = 1;

      // Remove request
      for (int j = i; j < MAX_FRIEND_REQUESTS - 1; j++) {
        accounts[cur_idx].friend_requests[j] =
            accounts[cur_idx].friend_requests[j + 1];
      }
      accounts[cur_idx].friend_requests[MAX_FRIEND_REQUESTS - 1] = 0;
      break;
    }
  }

  if (!request_found)
    return 2;

  // Add to friends lists
  int added_to_cur = 0, added_to_req = 0;

  for (int i = 0; i < MAX_FRIENDS; i++) {
    if (accounts[cur_idx].friends[i] == 0 && !added_to_cur) {
      accounts[cur_idx].friends[i] = accounts[requester_idx].id;
      added_to_cur = 1;
    }
    if (accounts[requester_idx].friends[i] == 0 && !added_to_req) {
      accounts[requester_idx].friends[i] = accounts[cur_idx].id;
      added_to_req = 1;
    }
    if (added_to_cur && added_to_req)
      break;
  }

  return 0;
}

int reject_friend_request(char *username) {
  int requester_idx = -1;
  int cur_idx = -1;

  // Find indices
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      requester_idx = i;
    }
  }

  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].logged) {
      for (int j = 0; j < count_accounts; j++) {
        if (strcmp(accounts[j].username, sessions[i].username) == 0) {
          cur_idx = j;
          break;
        }
      }
      break;
    }
  }

  if (requester_idx == -1 || cur_idx == -1)
    return 1;

  // Remove request
  for (int i = 0; i < MAX_FRIEND_REQUESTS; i++) {
    if (accounts[cur_idx].friend_requests[i] == accounts[requester_idx].id) {
      // Shift remaining requests
      for (int j = i; j < MAX_FRIEND_REQUESTS - 1; j++) {
        accounts[cur_idx].friend_requests[j] =
            accounts[cur_idx].friend_requests[j + 1];
      }
      accounts[cur_idx].friend_requests[MAX_FRIEND_REQUESTS - 1] = 0;
      return 0;
    }
  }

  return 2; // Request not found
}

int unfriend(char *username) {
  int target_idx = -1;
  int cur_idx = -1;

  // Find indices
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      target_idx = i;
    }
  }

  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].logged) {
      for (int j = 0; j < count_accounts; j++) {
        if (strcmp(accounts[j].username, sessions[i].username) == 0) {
          cur_idx = j;
          break;
        }
      }
      break;
    }
  }

  if (target_idx == -1 || cur_idx == -1)
    return 1;

  // Remove from both friends lists
  int removed_from_cur = 0, removed_from_target = 0;

  for (int i = 0; i < MAX_FRIENDS; i++) {
    if (accounts[cur_idx].friends[i] == accounts[target_idx].id) {
      // Shift remaining friends
      for (int j = i; j < MAX_FRIENDS - 1; j++) {
        accounts[cur_idx].friends[j] = accounts[cur_idx].friends[j + 1];
      }
      accounts[cur_idx].friends[MAX_FRIENDS - 1] = 0;
      removed_from_cur = 1;
    }
    if (accounts[target_idx].friends[i] == accounts[cur_idx].id) {
      // Shift remaining friends
      for (int j = i; j < MAX_FRIENDS - 1; j++) {
        accounts[target_idx].friends[j] = accounts[target_idx].friends[j + 1];
      }
      accounts[target_idx].friends[MAX_FRIENDS - 1] = 0;
      removed_from_target = 1;
    }
    if (removed_from_cur && removed_from_target)
      break;
  }

  return removed_from_cur && removed_from_target ? 0 : 2;
}
