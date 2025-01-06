#ifndef EVENT_H
#define EVENT_H
#include "config.h"

#define LEN_EVENT_NAME 50
#define LEN_EVENT_DATE 10
#define LEN_EVENT_ADDRESS 100
#define LEN_EVENT_DETAILS 500
#define MAX_MEMBERS 100
#define MAX_REQUESTS 100
#define MAX_EVENTS 1000

// For testing
#ifdef TEST
// #define TEST_db_read_events
#endif

typedef struct {
  int id;
  char name[LEN_EVENT_NAME + 1];
  char date[LEN_EVENT_DATE + 1];
  char address[LEN_EVENT_ADDRESS + 1];
  int type;
  char details[LEN_EVENT_DETAILS + 1];
  int owner;
  int members[MAX_MEMBERS];
  int requests[MAX_REQUESTS];
} event_t;

extern event_t events[MAX_EVENTS];
extern int count_events;
extern int gen_event_id;

int create_event(char *name, char *date, char *address, int type, char *details, int owner);
int update_event(int event_id, char *name, char *date, char *address, int type, char *details);
int delete_event(int event_id);
int get_event(char *src, char *name, char *date, char *address, int *type, char *details);


int get_event_idx(int event_id);
int get_event_owner(int event_id);
char *get_event_name(int event_id);
bool event_exists(int event_id);

int db_read_events();  // Tested
int db_save_events();  // Tested
int db_save_events0(); // Tested

#endif