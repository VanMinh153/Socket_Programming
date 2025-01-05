#include "server.h"

// pthread_mutex_t accounts_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
@function: handle message from client
@parameter: [IN] msg: message from client
            [IN] session: session's infomation
@return: 0 if success
         -1 if fail
         -2 if message not determine
*/
int handle_msg(char *msg, session_t *_session) {
  session_t session = *_session;
  int connfd = session.connfd;
  // char *recv_buffer = session.recv_buffer;
  char *parse = msg;
  int retval = -1;
  char send_m[LEN_MSG + 1] = {0};
  char str_temp[LEN_MSG + 1] = {0}; // temporary string
  int temp = 0;

  char command[LEN_COMMAND + 1] = {0};
  char option[LEN_OPTION + 1] = {0};

  //@@
  printf("\n### %d: `%s`\n", connfd, msg);

  // Get command
  retval = sscanf_(&parse, command);
  if (retval == 1) {
    send_(BLANK_MSG);
    return 1;
  } else if (retval == 2) {
    send_(COMMAND_NOT_FOUND);
    return 1;
  }

  //@@
  // printf("Command: `%s`\n", command);
  // send_(command);
  printf("***\n");

  //________________________________________________________________________________________
  int user_id = session.user_id;
  int friend_id = -1;
  int event_id = -1;
  // int user_idx = -1;
  int friend_idx = -1;
  int event_idx = -1;
  // int access_flag = 0;

  char username[LEN_USERNAME + 1] = {0};
  char password[LEN_PASSWORD + 1] = {0};
  char event[LEN_EVENT_NAME + 1] = {0};
  //______________________________________________________________________
  // signin 'username' 'password'
  if (strcmp(command, "signin") == 0) {
    if (session.user_id != 0) {
      send_(SIGNED_IN);
      fprintf(stderr, "User has been logged in\n");
      return 2;
    }

    retval = sscanf_(&parse, username);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(USER_NOT_FOUND);
      return 1;
    }

    retval = sscanf_(&parse, password);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(PASSWORD_INCORRECT);
      return 1;
    }

    if (!is_blank(parse)) {
      send_(ARGS_EXCEED);
      return 1;
    }

    user_id = get_user_id(username);
    if (user_id == -1) {
      send_(ACCOUNT_NOT_EXIST);
      return 1;
    }

    retval = handle_signin(user_id, password);
    if (retval == 2) {
      send_(PASSWORD_INCORRECT);
      return 1;
    }
    assert(retval == 0);
    _session->user_id = get_user_id(username);
    send_(OK);
  }
  //______________________________________________________________________
  // signup 'username' 'password'
  else if (strcmp(command, "signup") == 0) {
    retval = sscanf_(&parse, username);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(USER_NOT_FOUND);
      return 1;
    }

    retval = sscanf_(&parse, password);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(PASSWORD_INCORRECT);
      return 1;
    }

    if (!is_blank(parse)) {
      send_(ARGS_EXCEED);
      return 1;
    }

    retval = handle_signup(username, password);
    if (retval == 1) {
      send_(OUT_OF_MEMORY);
    } else if (retval == 2) {
      send_(USERNAME_FORMAT_INCORRECT);
      return 1;
    } else if (retval == 3) {
      send_(PASSWORD_FORMAT_INCORRECT);
      return 1;
    } else if (retval == 4) {
      send_(ACCOUNT_EXISTED);
      return 1;
    }
    send_(OK);
  }
  //______________________________________________________________________
  // Only login state can use this command
  if (session.user_id == 0) {
    send_(NOT_SIGNED_IN);
    return 1;
  } else {
    // user_idx = get_user_idx(user_id);
  }
  //______________________________________________________________________
  // signout
  if (strcmp(command, "signout") == 0) {
    if (!is_blank(parse)) {
      send_(ARGS_EXCEED);
      return 1;
    }
    _session->user_id = 0;
    send_(OK);
  }
  //______________________________________________________________________
  // change_password 'current_password' 'new_password'
  else if (strcmp(command, "change_password") == 0) {
    retval = sscanf_(&parse, password);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(PASSWORD_FORMAT_INCORRECT);
      return 1;
    }

    retval = sscanf_2(&parse, str_temp, LEN_PASSWORD);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(PASSWORD_FORMAT_INCORRECT);
      return 1;
    }

    if (!is_blank(parse)) {
      send_(ARGS_EXCEED);
      return 1;
    }

    retval = handle_change_password(user_id, password, str_temp);
    if (retval == 2) {
      send_(PASSWORD_INCORRECT);
      return 1;
    } else if (retval == 3) {
      send_(PASSWORD_FORMAT_INCORRECT);
      return 1;
    }
    assert(retval == 0);
    send_(OK);
  }
  //______________________________________________________________________
  // change_username 'password' 'new_username'
  else if (strcmp(command, "change_username") == 0) {
    retval = sscanf_(&parse, password);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(PASSWORD_FORMAT_INCORRECT);
      return 1;
    }

    retval = sscanf_(&parse, username);
    if (retval == 1) {
      send_(ARGS_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(USERNAME_FORMAT_INCORRECT);
      return 1;
    }

    if (!is_blank(parse)) {
      send_(ARGS_EXCEED);
      return 1;
    }

    retval = handle_change_username(user_id, password, str_temp);
    if (retval == 2) {
      send_(PASSWORD_INCORRECT);
      return 1;
    } else if (retval == 3) {
      send_(USERNAME_FORMAT_INCORRECT);
      return 1;
    }
    assert(retval == 0);
    send_(OK);
  }
  //______________________________________________________________________
  // read -user | -event | -event_info
  else if (strcmp(command, "read") == 0) {
    retval = sscanf_(&parse, option);
    if (retval == 1) {
      send_(OPTION_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(OPTION_INVALID);
      return 1;
    }
    //______________________________________________________________________
    // read -user
    if (strcmp(option, "-user") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_read_user(user_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // read -event 'event_id'
    else if (strcmp(str_temp, "-event") == 0) {
      retval = sscanf_d(&parse, &event_id);
      if (retval == 1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      event_idx = get_event_idx(event_id);
      if (event_idx == -1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }
      if (events[event_idx].owner != user_id) {
        send_(NO_ACCESS);
        return 1;
      }

      retval = handle_read_event(event_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // read -event_info 'event_id'
    else if (strcmp(str_temp, "-event_info") == 0) {
      retval = sscanf_d(&parse, &event_id);
      if (retval == 1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      event_idx = get_event_idx(event_id);
      if (event_idx == -1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }
      if (events[event_idx].type == 1) {
        if (!existed(events[event_idx].members, MAX_MEMBERS, user_id)) {
          send_(NO_ACCESS);
          return 1;
        }
      }

      retval = handle_read_event_info(event_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    // [command] read
    else {
      if (*option != '-') {
        send_(OPTION_MISSING);
        return 1;
      } else {
        send_(OPTION_INVALID);
        return 1;
      }
    }
  }
  //______________________________________________________________________
  // list -user | -event | -event_detail
  else if (strcmp(command, "list") == 0) {
    retval = sscanf_(&parse, option);
    if (retval == 1) {
      send_(OPTION_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(OPTION_INVALID);
      return 1;
    }
    //______________________________________________________________________
    // list -user
    if (strcmp(option, "-user") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_list_user();
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // list -event
    else if (strcmp(option, "-event") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_list_event();
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // list -event_detail
    else if (strcmp(option, "-event_detail") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_list_event_detail();
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // list -event_member 'event_id'
    else if (strcmp(option, "-event_member") == 0) {
      retval = sscanf_d(&parse, &event_id);
      if (retval == 1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      event_idx = get_event_idx(event_id);
      if (event_idx == -1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }
      if (events[event_idx].owner != user_id) {
        send_(NO_ACCESS);
        return 1;
      }

      retval = handle_eventM_event_member(event_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // list -event_request 'event_id'
    else if (strcmp(option, "-event_request") == 0) {
      retval = sscanf_d(&parse, &event_id);
      if (retval == 1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      event_idx = get_event_idx(event_id);
      if (event_idx == -1) {
        send_(EVENT_NOT_FOUND);
        return 1;
      }
      if (events[event_idx].owner != user_id) {
        send_(NO_ACCESS);
        return 1;
      }

      retval = handle_eventO_event_request(event_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    // [command] list
    else {
      if (*option != '-') {
        send_(OPTION_MISSING);
        return 1;
      } else {
        send_(OPTION_INVALID);
        return 1;
      }
    }
  }
  //______________________________________________________________________
  // friend -request | -take_back | -accept | -reject | -unfriend 
  else if (strcmp(command, "friend") == 0) {
    retval = sscanf_(&parse, option);
    if (retval == 1) {
      send_(OPTION_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(OPTION_INVALID);
      return 1;
    }
    //______________________________________________________________________
    // friend -request 'friend_id'
    if (strcmp(option, "-request") == 0) {
      retval = sscanf_d(&parse, &friend_id);
      if (retval == 1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      friend_idx = get_user_idx(friend_id);
      if (friend_idx == -1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      retval = handle_friend_request(user_id, friend_id);
      if (retval == 2) {
        send_(REQUEST_EXISTED);
        return 1;
      } else if (retval == 3) {
        send_(REACHED_LIMIT);
        return 1;
      }
      assert(retval == 0);
      send_(OK);
      return 0;
    }
    //______________________________________________________________________
    // friend -take_back 'friend_id'
    else if (strcmp(option, "-take_back") == 0) {
      retval = sscanf_d(&parse, &friend_id);
      if (retval == 1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      friend_idx = get_user_idx(friend_id);
      if (friend_idx == -1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      retval = handle_friend_take_back(user_id, friend_id);
      if (retval == 2) {
        send_(REQUEST_NOT_FOUND);
        return 1;
      }
      assert(retval == 0);
      send_(OK);
      return 0;
    }
    //______________________________________________________________________
    // friend -accept 'friend_id'
    else if (strcmp(option, "-accept") == 0) {
      retval = sscanf_d(&parse, &friend_id);
      if (retval == 1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      friend_idx = get_user_idx(friend_id);
      if (friend_idx == -1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      retval = handle_friend_accept(user_id, friend_id);
      if (retval == 2) {
        send_(REQUEST_NOT_FOUND);
        return 1;
      } else if (retval == 4) {
        send_(REACHED_LIMIT);
        return 1;
      }
      assert(retval == 0);
      send_(OK);
      return 0;
    }
    //______________________________________________________________________
    // friend -reject 'friend_id'
    else if (strcmp(option, "-reject") == 0) {
      retval = sscanf_d(&parse, &friend_id);
      if (retval == 1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      friend_idx = get_user_idx(friend_id);
      if (friend_idx == -1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      retval = handle_friend_reject(user_id, friend_id);
      if (retval == 2) {
        send_(REQUEST_NOT_FOUND);
        return 1;
      }
      assert(retval == 0);
      send_(OK);
      return 0;
    }
    //______________________________________________________________________
    // friend -unfriend 'friend_id'
    else if (strcmp(option, "-unfriend") == 0) {
      retval = sscanf_d(&parse, &friend_id);
      if (retval == 1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      friend_idx = get_user_idx(friend_id);
      if (friend_idx == -1) {
        send_(USER_NOT_FOUND);
        return 1;
      }

      retval = handle_friend_unfriend(user_id, friend_id);
      if (retval == 2) {
        send_(NOT_FRIEND);
        return 1;
      }
      assert(retval == 0);
      send_(OK);
      return 0;
    }
    // [command] friend
    else {
      if (*option != '-') {
        send_(OPTION_MISSING);
        return 1;
      } else {
        send_(OPTION_INVALID);
        return 1;
      }
    }
  }
  //______________________________________________________________________
  // event -create | -update | -delete | -join | -leave | -request | -accept | -reject
  // event -event_member | -event_request
  else if (strcmp(command, "event") == 0) {
    char name[LEN_EVENT_NAME + 1] = {0};
    char date[LEN_EVENT_DATE + 1] = {0};
    char address[LEN_EVENT_ADDRESS + 1] = {0};
    int type = 0;
    char details[LEN_EVENT_DETAILS + 1] = {0};
    // int owner = user_id;
    
    retval = sscanf_(&parse, option);
    if (retval == 1) {
      send_(OPTION_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(OPTION_INVALID);
      return 1;
    }
    //______________________________________________________________________
    // event -create 'event_name' 'event_date' 'event_address' 'event_type' 'event_details'
    if (strcmp(option, "-create") == 0) {
    }
    //______________________________________________________________________
    // event -update 'event_id' 'event_name' 'event_date' 'event_address' 'event_type' 'event_details'
    else if (strcmp(option, "-update") == 0) {
    }
    // [command] event
    else {
      if (*option != '-') {
        send_(OPTION_MISSING);
        return 1;
      } else {
        send_(OPTION_INVALID);
        return 1;
      }
    }
  }
  //______________________________________________________________________
  // user -friend | -friend_request | -event_join | -event_request
  else if (strcmp(command, "user") == 0) {
    retval = sscanf_(&parse, option);
    if (retval == 1) {
      send_(OPTION_MISSING);
      return 1;
    } else if (retval == 2) {
      send_(OPTION_INVALID);
      return 1;
    }
    //______________________________________________________________________
    // user -friend
    if (strcmp(option, "-friend") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_user_friend(user_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // user -friend_request
    else if (strcmp(option, "-friend_request") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_user_friend_request(user_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // user -event_join
    else if (strcmp(option, "-event_join") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_user_event_join(user_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    //______________________________________________________________________
    // user -event_request
    else if (strcmp(option, "-event_request") == 0) {
      if (!is_blank(parse)) {
        send_(ARGS_EXCEED);
        return 1;
      }

      retval = handle_user_event_request(user_id);
      assert(retval == 0);
      send_(OK);
      send_(cmd_return);
      return 0;
    }
    // [command] user
    else {
      if (*option != '-') {
        send_(OPTION_MISSING);
        return 1;
      } else {
        send_(OPTION_INVALID);
        return 1;
      }
    }
  }
  //______________________________________________________________________

  send_(COMMAND_NOT_FOUND);
  return 2;
}
