#ifndef COMMAND_H
#define COMMAND_H

#include "account.h"
#include "config.h"
#include "event.h"
#include "session.h"
#include "utils.h"
#include <assert.h>

extern char cmd_return[LEN_MSG + 1];

// for all user
int handle_read_event_info(int event);

int handle_list_user();
int handle_list_event();
int handle_list_event_detail();

// for current user
int handle_read_user(int user);

int handle_user_friend(int user);
int handle_user_friend_request(int user);
int handle_user_event_join(int user);
int handle_user_event_request(int user);

// handle list command for admin
int handle_listA_user();
int handle_listA_event();
int handle_listA_session();
int handle_listA_log();

// account management
int handle_signin(int user, char *password);
int handle_signup(char *username, char *password);
int handle_signout(int user);
int handle_change_password(int user, char *cur_password, char *new_password);
int handle_change_username(int user, char *password, char *new_username);

// friend management
int handle_friend_request(int user, int friend);
int handle_friend_take_back(int user, int friend);
int handle_friend_accept(int user, int friend);
int handle_friend_reject(int user, int friend);
int handle_friend_unfriend(int user, int friend);

// for all users
int handle_event_create(int user, char *event_name, char *date, char *address,
                        int type, char *details);

int handle_event_request(int user, int event);
int handle_event_take_back(int user, int event);
int handle_event_accept(int user, int event);
int handle_event_reject(int user, int event);

// for event's member
int handle_eventM_event_member(int event);
int handle_eventM_request(int user, int friend, int event);
int handle_eventM_take_back(int user, int friend, int event);

// for event's owner
int handle_read_event(int event);

int handle_eventO_update(int event, char *event_name, char *date, char *address,
                        int type, char *details);
int handle_eventO_delete(int event);
int handle_eventO_event_request(int event);
int handle_eventO_accept(int user, int event);
int handle_eventO_reject(int user, int event);
#endif