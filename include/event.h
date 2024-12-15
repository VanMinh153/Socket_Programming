#ifndef EVENT_H
#define EVENT_H

#include <time.h>

typedef enum {
    PRIVATE,
    PUBLIC
} EventType;

typedef struct {
    char name[100];
    time_t timestamp;
    char location[100];
    EventType type;
    char description[500];
    char* invited_users[100];
    char* participated_users[100];
    int invited_count;
    int participated_count;
} Event;

int handle_create_event(client_info* client, char* event_details);
int list_events(client_info* client);
int update_event(client_info* client, char* event_details);
int delete_event(client_info* client, int event_id);
int invite_to_event(const char* event_name, const char* inviter, const char* invited_user);
int request_event_participation(const char* event_name, const char* requester);

#endif