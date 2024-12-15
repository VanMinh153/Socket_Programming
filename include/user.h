#ifndef USER_H
#define USER_H

#include "server.h"

typedef struct {
    char username[50];
    char password[50];
    char* friends[100];
    int friend_count;
} User;

int handle_login(client_info* client, char* credentials);
int handle_registration(client_info* client, char* user_info);
int send_friend_request(const char* from_user, const char* to_user);
int accept_friend_request(const char* from_user, const char* to_user);
int remove_friend(const char* user, const char* friend_to_remove);

#endif