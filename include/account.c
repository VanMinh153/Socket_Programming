#include "account.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

// Global variables
account_t accounts[MAX_ACCOUNTS];
int count_accounts = 0;
int gen_account_id = 1;

// Helper function to find account by username
static int find_account_by_username(const char* username) {
  for (int i = 0; i < count_accounts; i++) {
    if (strcmp(accounts[i].username, username) == 0) {
      return i;
    }
  }
  return -1;
}

int handle_login(char* username, char* password) {
  int acc_idx = find_account_by_username(username);
  
  if (acc_idx == -1) {
    return 2; // Username not found
  }
  
  if (strcmp(accounts[acc_idx].password, password) == 0) {
    return 0; // Login successful
  }
  
  return 1; // Incorrect password
}

int handle_signup(char* username, char* password) {
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

int handle_change_password(char* username, char* cur_password, char* new_password) {
  int acc_idx = find_account_by_username(username);
  
  if (acc_idx != -1 && strcmp(accounts[acc_idx].password, cur_password) == 0) {
    strncpy(accounts[acc_idx].password, new_password, LEN_PASSWORD);
    return 0;
  }
  
  return 1; // Current password incorrect
}

int check_format_username(char* username) {
  regex_t regex;
  int ret;
  
  // Compile regex pattern for username
  ret = regcomp(&regex, USERNAME_REGEX, REG_EXTENDED);
  if (ret) {
    return 1; // Regex compilation failed
  }
  
  // Execute regex match
  ret = regexec(&regex, username, 0, NULL, 0);
  regfree(&regex);
  
  if (ret == 0) {
    return 0; // Format is correct
  }
  return 1; // Format is incorrect
}

int check_format_password(char* password) {
  // Check if password contains only alphanumeric characters
  for (int i = 0; password[i] != '\0'; i++) {
    if (!isalnum(password[i])) {
      return 1;
    }
  }
  return 0;
}

int db_save_accounts() {
  FILE* fp = fopen(DB_ACCOUNTS, "w");
  if (!fp) {
    return 1;
  }
  
  for (int i = 0; i < count_accounts; i++) {
    // Write ID, username, and password
    fprintf(fp, "%d | %s | %s |", 
        accounts[i].id, 
        accounts[i].username, 
        accounts[i].password);
    
    // Write friends
    for (int j = 0; j < MAX_FRIENDS && accounts[i].friends[j] != 0; j++) {
      fprintf(fp, " %d", accounts[i].friends[j]);
    }
    fprintf(fp, " | ");
    
    // Write friend requests
    for (int j = 0; j < MAX_FRIEND_REQUESTS && accounts[i].friend_requests[j] != 0; j++) {
      fprintf(fp, " %d", accounts[i].friend_requests[j]);
    }
    fprintf(fp, " | ");
        
    // Write event joins
    for (int j = 0; j < MAX_EVENT_JOINS && accounts[i].event_joins[j] != 0; j++) {
      fprintf(fp, " %d", accounts[i].event_joins[j]);
    }
    fprintf(fp, " | ");
    
    // Write event requests
    for (int j = 0; j < MAX_EVENT_REQUESTS && accounts[i].event_requests[j] != 0; j++) {
      fprintf(fp, " %d", accounts[i].event_requests[j]);
    }
    fprintf(fp, "\n");
  }
  
  fclose(fp);
  return 0;
}


