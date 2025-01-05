#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "config.h"

#define LEN_USERNAME 30
#define LEN_PASSWORD 30
#define MAX_FRIENDS 100
#define MAX_FRIEND_REQUESTS 50
#define MAX_EVENT_JOINS 100
#define MAX_EVENT_REQUESTS 50
#define MAX_ACCOUNTS 1000
#define USERNAME_REGEX "^[a-zA-Z0-9]+(-[a-zA-Z0-9]+)*$"

// For testing
#ifdef TEST
// #define TEST_db_read_accounts
#endif

typedef struct {
    int id;
    char username[LEN_USERNAME+1];
    char password[LEN_PASSWORD+1];
    int friends[MAX_FRIENDS];
    int friend_requests[MAX_FRIEND_REQUESTS];
    int event_joins[MAX_EVENT_JOINS];
    int event_requests[MAX_EVENT_REQUESTS][2];
} account_t;

extern account_t accounts[MAX_ACCOUNTS];
extern int count_accounts;
extern int gen_account_id;

int create_account(char *username, char *password);
int get_user_id(char *username);
int get_user_idx(int user_id);
char *get_username(int user_id);
int check_password(int user_id, char *password);
bool user_exists(int user_id);

int check_format_username(char* username); // Tested 
int check_format_password(char* password); // Tested
int db_read_accounts(); // Tested
int db_save_accounts(); // Tested
int db_save_accounts0(); // Tested

#endif
