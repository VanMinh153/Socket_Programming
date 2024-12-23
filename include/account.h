#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "server.h"

#define USERNAME_LEN 30
#define PASSWORD_LEN 30
#define MAX_FRIEND 100
#define MAX_ACCOUNT 1000

#define A 5

typedef struct {
    char username[31];
    char password[31];
    char friends[100][31];
    int friend_count;
} account_t;

// int handle_login(client_info* client, char* credentials);
// int handle_registration(client_info* client, char* user_info);
// int send_friend_request(const char* from_user, const char* to_user);
// int accept_friend_request(const char* from_user, const char* to_user);
// int remove_friend(const char* user, const char* friend_to_remove);

#endif