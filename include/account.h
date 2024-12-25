#ifndef ACCOUNT_H
#define ACCOUNT_H

#define LEN_USERNAME 30
#define LEN_PASSWORD 30
#define MAX_FRIENDS 100
#define MAX_FRIEND_REQUESTS 50
#define MAX_FRIEND_INVITES 50
#define MAX_EVENT_JOINS 100
#define MAX_EVENT_REQUESTS 50
#define MAX_EVENT_INVITES 50
#define MAX_ACCOUNTS 1000
#define USERNAME_REGEX "^[a-zA-Z0-9]+(-[a-zA-Z0-9]+)*$"

typedef struct {
    int id;
    char username[LEN_USERNAME+1];
    char password[LEN_PASSWORD+1];
    int friends[MAX_FRIENDS];
    int friend_requests[MAX_FRIEND_REQUESTS];
    int friend_invites[MAX_FRIEND_INVITES];
    int event_joins[MAX_EVENT_JOINS];
    int event_requests[MAX_EVENT_REQUESTS];
    int event_invites[MAX_EVENT_INVITES][2];
} account_t;

extern account_t accounts[MAX_ACCOUNTS];
extern int count_accounts;
extern int gen_account_id;

int handle_login(char* username, char *password);
int handle_signup(char* username, char *password);
int handle_change_password(char* username, char* cur_password, char *new_password);
int check_format_username(char* username);
int check_format_password(char* password);
int db_read_accounts();
int db_save_accounts(account_t* accounts);

// Friend management functions
int create_friend_invites(char* username);
int take_back_friend_invites(char* username);
int accept_friend_request(char* username);
int reject_friend_request(char* username);
int unfriend(char* username);

#endif
