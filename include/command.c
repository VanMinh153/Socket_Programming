#include "command.h"

char cmd_return[LEN_MSG + 1] = {0};

//_________________________________________________________________________________________
// for current user: account management
int handle_signin(int user, char *password) {
  int retval = check_password(user, password);
  if (retval != 0)
    return retval;
  return 0;
}

int handle_signup(char *username, char *password) {
  int retval = create_account(username, password);
  if (retval != 0)
    return retval;
  return 0;
}

int handle_signout(int user) { return 0; }

int handle_change_password(int user_id, char *cur_password,
                           char *new_password) {
  int retval = check_password(user_id, cur_password);
  if (retval != 0)
    return retval;

  if (check_format_password(new_password) == 1)
    return 3;

  int user_idx = get_user_idx(user_id);
  memcpy(accounts[user_idx].password, new_password, strlen(new_password) + 1);
  return 0;
}

int handle_change_username(int user_id, char *password, char *new_username) {
  int retval = check_password(user_id, password);
  if (retval != 0)
    return retval;

  if (check_format_username(new_username) == 1)
    return 3;

  int user_idx = get_user_idx(user_id);
  memcpy(accounts[user_idx].username, new_username, strlen(new_username) + 1);
  return 0;
}

//_________________________________________________________________________________________
// for all user
int handle_read_event_info(int event) {
  int event_idx = get_event_idx(event);
  if (event_idx == -1)
    return 1;
  sprintf(cmd_return, " %s | %s | %s | %d | %s | %d", events[event_idx].name,
          events[event_idx].date, events[event_idx].address,
          events[event_idx].type, events[event_idx].details,
          events[event_idx].owner);
  return 0;
}

int handle_list_user() {
  for (int i = 0; i < count_accounts; i++)
    sprintf(cmd_return, " %s", accounts[i].username);
  return 0;
}

int handle_list_event() {
  for (int i = 0; i < count_events; i++)
    sprintf(cmd_return, " %d | %s $", events[i].id, events[i].name);
  last_char(cmd_return, '\0');
  return 0;
}

int handle_list_event_detail() {
  for (int i = 0; i < count_events; i++) {
    sprintf(cmd_return, " %d | %s | %s | %s | %d | %s | %d $", events[i].id,
            events[i].name, events[i].date, events[i].address, events[i].type,
            events[i].details, events[i].owner);
  }
  last_char(cmd_return, '\0');
  return 0;
}

//_________________________________________________________________________________________
// for admin
int handle_listA_user() { return 0; }

int handle_listA_event() { return 0; }

int handle_listA_session() { return 0; }

int handle_listA_log() { return 0; }

//_________________________________________________________________________________________
// for current user
int handle_user_read_user(int user_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;
  for (int i = 0; i < count_accounts; i++) {
    if (accounts[i].id == user_id)
      user_idx = i;
  }

  for (int i = 0; i < MAX_FRIENDS && accounts[user_idx].friends[i] != 0; i++)
    sprintf(cmd_return, " %s", get_username(accounts[user_idx].friends[i]));

  strcat(cmd_return, " |");

  for (int i = 0;
       i < MAX_FRIEND_REQUESTS && accounts[user_idx].friend_requests[i] != 0;
       i++)
    sprintf(last_of(cmd_return), " %s",
            get_username(accounts[user_idx].friend_requests[i]));

  strcat(cmd_return, " |");

  for (int i = 0; i < MAX_EVENT_JOINS && accounts[user_idx].event_joins[i] != 0;
       i++)
    sprintf(last_of(cmd_return), " %s $",
            get_event_name(accounts[user_idx].event_joins[i]));

  last_char(cmd_return, '|');

  for (int i = 0;
       i < MAX_EVENT_REQUESTS && accounts[user_idx].event_requests[i][0] != 0;
       i++)
    sprintf(last_of(cmd_return), " %s,%s $",
            get_username(accounts[user_idx].event_requests[i][0]),
            get_event_name(accounts[user_idx].event_requests[i][1]));

  last_char(cmd_return, '\0');
  return 0;
}

int handle_user_friend(int user_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;
  for (int i = 0; i < MAX_FRIENDS && accounts[user_idx].friends[i] != 0; i++)
    sprintf(cmd_return, " %s", get_username(accounts[user_idx].friends[i]));
  return 0;
}

int handle_user_friend_request(int user_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;
  for (int i = 0;
       i < MAX_FRIEND_REQUESTS && accounts[user_idx].friend_requests[i] != 0;
       i++)
    sprintf(cmd_return, " %s",
            get_username(accounts[user_idx].friend_requests[i]));
  return 0;
}

int handle_user_event_join(int user_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;
  for (int i = 0; i < MAX_EVENT_JOINS && accounts[user_idx].event_joins[i] != 0;
       i++)
    sprintf(cmd_return, " %s |",
            get_event_name(accounts[user_idx].event_joins[i]));
  last_char(cmd_return, '\0');
  return 0;
}

int handle_user_event_request(int user_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;
  for (int i = 0;
       i < MAX_EVENT_REQUESTS && accounts[user_idx].event_requests[i][0] != 0;
       i++)
    sprintf(cmd_return, " %s,%s |",
            get_username(accounts[user_idx].event_requests[i][0]),
            get_event_name(accounts[user_idx].event_requests[i][1]));
  last_char(cmd_return, '\0');
  return 0;
}

//_________________________________________________________________________________________
// for current user: friend management
int handle_friend_request(int user_id, int friend_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  int retval = add_element(accounts[friend_idx].friend_requests,
                           MAX_FRIEND_REQUESTS, user_id);
  if (retval != 0)
    return retval + 1;
  return 0;
}

int handle_friend_take_back(int user_id, int friend_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  if (!remove_element(accounts[friend_idx].friend_requests, MAX_FRIEND_REQUESTS,
                      friend_id))
    return 2;
  return 0;
}

int handle_friend_accept(int user_id, int friend_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  if (!remove_element(accounts[user_idx].friend_requests, MAX_FRIEND_REQUESTS,
                      friend_id))
    return 2;

  int retval = add_element(accounts[user_idx].friends, MAX_FRIENDS, friend_id);
  int retval2 = add_element(accounts[friend_idx].friends, MAX_FRIENDS, user_id);
  if (retval == 2 || retval2 == 2)
    return 3;
  if (retval == 1 && retval2 == 1) {
    fprintf(
        stderr,
        "handle_friend_accept(): database inconsistency: already a friend\n");
    return 4;
  }
  if (retval != 1 || retval2 != 1) {
    fprintf(stderr, "handle_friend_accept(): database inconsistency: be "
                    "friends on one side\n");
    return 5;
  }
  assert(retval == 0 && retval2 == 0);
  return 0;
}

int handle_friend_reject(int user_id, int friend_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  if (!remove_element(accounts[user_idx].friend_requests, MAX_FRIEND_REQUESTS,
                      friend_id))
    return 2;
  return 0;
}

int handle_friend_unfriend(int user_id, int friend_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  bool retval =
      remove_element(accounts[user_idx].friends, MAX_FRIENDS, friend_id);
  bool retval2 =
      remove_element(accounts[friend_idx].friends, MAX_FRIENDS, user_id);
  if (!retval && !retval2)
    return 2;
  if (!retval || !retval2) {
    fprintf(stderr, "handle_friend_unfriend(): database inconsistency: be "
                    "friends on one side\n");
    return 3;
  }
  assert(retval && retval2);
  return 0;
}

//_________________________________________________________________________________________
// for all users: event management
int handle_event_create(int user_id, char *event_name, char *date,
                        char *address, int type, char *details) {
  int retval = create_event(event_name, date, address, type, details, user_id);
  if (retval != 0)
    return retval;
  return 0;
}

int handle_event_request(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  int retval = add_element(events[event_idx].requests, MAX_REQUESTS, user_id);
  if (retval != 0)
    return retval + 1;
  return 0;
}

int handle_event_take_back(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  if (!remove_element(events[event_idx].requests, MAX_REQUESTS, user_id))
    return 2;
  return 0;
}

int handle_event_accept(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  if (remove_element2e2(accounts[user_idx].event_requests, MAX_EVENT_REQUESTS,
                        event_id) == 0)
    return 2;

  int retval = add_element(events[event_idx].members, MAX_MEMBERS, user_id);
  if (retval != 0)
    return retval + 2;
  return 0;
}

int handle_event_reject(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  if (remove_element2e2(accounts[user_idx].event_requests, MAX_EVENT_REQUESTS,
                        event_id) == 0)
    return 2;
  return 0;
}

//_________________________________________________________________________________________
// for event's member
int handle_eventM_event_member(int event) {
  int event_idx = get_event_idx(event);
  if (event_idx == -1)
    return 1;
  for (int i = 0; i < MAX_MEMBERS && events[event_idx].members[i] != 0; i++)
    sprintf(cmd_return, " %s", get_username(events[event_idx].members[i]));
  return 0;
}

/**
 * Return 0 if success
 *       1 if user not found or friend not found
 *       2 if event not found
 */
int handle_eventM_request(int user_id, int friend_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 2;

  int retval = add_element2(accounts[friend_idx].event_requests,
                            MAX_EVENT_REQUESTS, user_id, event_id);
  if (retval != 0)
    return retval + 2;
  return 0;
}

int handle_eventM_take_back(int user_id, int friend_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int friend_idx = get_user_idx(friend_id);
  if (friend_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 2;

  if (!remove_element2(accounts[friend_idx].event_requests, MAX_EVENT_REQUESTS,
                       user_id, event_id))
    return 3;
  return 0;
}

//_________________________________________________________________________________________
// for event's owner
int handle_eventO_read_event(int event_id) {
  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;
  sprintf(cmd_return, " %s | %s | %s | %d | %s | %d", events[event_idx].name,
          events[event_idx].date, events[event_idx].address,
          events[event_idx].type, events[event_idx].details,
          events[event_idx].owner);

  for (int i = 0; i < MAX_MEMBERS && events[event_idx].members[i] != 0; i++)
    sprintf(cmd_return, " %s", get_username(events[event_idx].members[i]));

  strcat(cmd_return, " |");

  for (int i = 0; i < MAX_REQUESTS && events[event_idx].requests[i] != 0; i++)
    sprintf(last_of(cmd_return), " %s",
            get_username(events[event_idx].requests[i]));
  return 0;
}

int handle_eventO_update(int event_id, char *event_name, char *date,
                         char *address, int type, char *details) {
  int retval = update_event(event_id, event_name, date, address, type, details);
  if (retval != 0)
    return retval;
  return 0;
}

int handle_eventO_delete(int event_id) {
  int retval = delete_event(event_id);
  if (retval != 0)
    return retval;
  return 0;
}

int handle_eventO_event_request(int event_id) {
  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;
  for (int i = 0; i < MAX_REQUESTS && events[event_idx].requests[i] != 0; i++)
    sprintf(cmd_return, " %s", get_username(events[event_idx].requests[i]));
  return 0;
}

int handle_eventO_accept(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  if (!remove_element(events[event_idx].requests, MAX_REQUESTS, user_id))
    return 2;

  int retval = add_element(events[event_idx].members, MAX_MEMBERS, user_id);
  if (retval != 0)
    return retval + 2;
  return 0;
}

int handle_eventO_reject(int user_id, int event_id) {
  int user_idx = get_user_idx(user_id);
  if (user_idx == -1)
    return 1;

  int event_idx = get_event_idx(event_id);
  if (event_idx == -1)
    return 1;

  if (!remove_element(events[event_idx].requests, MAX_REQUESTS, user_id))
    return 2;
  return 0;
}