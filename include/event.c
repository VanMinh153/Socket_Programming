#include "event.h"
#include "session.h"

// Global variables
event_t events[MAX_EVENTS] = {0};
int count_events = 0;
int gen_event_id = 1;

// bool check_event_member()

int create_event(char *name, char *date, char *address, int type, char *details, int owner) {
  if (count_events >= MAX_EVENTS) {
    fprintf(stderr, "Event database is full\n");
    return 1;
  }

  if (strlen(name) > LEN_EVENT_NAME) {
    fprintf(stderr, "Event name too long: \"%s\"\n", name);
    return 2;
  }
  if (strlen(date) > LEN_EVENT_DATE) {
    fprintf(stderr, "Event date too long: \"%s\"\n", date);
    return 3;
  }
  if (strlen(address) > LEN_EVENT_ADDRESS) {
    fprintf(stderr, "Event address too long: \"%s\"\n", address);
    return 4;
  }
  if (type != 0 && type != 1) {
    fprintf(stderr, "Invalid event type: %d\n", type);
    return 5;
  }
  if (strlen(details) > LEN_EVENT_DETAILS) {
    fprintf(stderr, "Event details too long: \"%s\"\n", details);
    return 6;
  }
  if (get_user_idx(owner) == -1) {
    fprintf(stderr, "Owner ID not found: %d\n", owner);
    return 7;
  }

  // Create new event
  events[count_events].id = gen_event_id;
  memcpy(events[count_events].name, name, strlen(name) + 1);
  memcpy(events[count_events].date, date, strlen(date) + 1);
  memcpy(events[count_events].address, address, strlen(address) + 1);
  events[count_events].type = type;
  memcpy(events[count_events].details, details, strlen(details) + 1);
  events[count_events].owner = owner;
  events[count_events].members[0] = owner;
  gen_event_id++;
  count_events++;
  return 0;
}

int update_event(int event_id, char *name, char *date, char *address, int type, char *details) {
  int idx = get_event_idx(event_id);
  if (idx == -1)
    return 1;
  
  if (strlen(name) > LEN_EVENT_NAME) {
    fprintf(stderr, "Event name too long: \"%s\"\n", name);
    return 2;
  }
  if (strlen(date) > LEN_EVENT_DATE) {
    fprintf(stderr, "Event date too long: \"%s\"\n", date);
    return 3;
  }
  if (strlen(address) > LEN_EVENT_ADDRESS) {
    fprintf(stderr, "Event address too long: \"%s\"\n", address);
    return 4;
  }
  if (type != 0 && type != 1) {
    fprintf(stderr, "Invalid event type: %d\n", type);
    return 5;
  }
  if (strlen(details) > LEN_EVENT_DETAILS) {
    fprintf(stderr, "Event details too long: \"%s\"\n", details);
    return 6;
  }

  memcpy(events[idx].name, name, strlen(name) + 1);
  memcpy(events[idx].date, date, strlen(date) + 1);
  memcpy(events[idx].address, address, strlen(address) + 1);
  events[idx].type = type;
  memcpy(events[idx].details, details, strlen(details) + 1);
  return 0;
}

int delete_event(int event_id) {
  int idx = get_event_idx(event_id);
  if (idx == -1)
    return 1;

  int idx2 = -1;
  for (int i = 0; i < count_events; i++) {
    if (events[i].id == 0) {
      idx2 = i;
      break;
    }
  }
  if (idx2 == -1) 
    idx2 = count_events - 1;
  
  events[idx] = events[idx2];
  events[idx2].id = 0;
  return 0;
}

int get_event_idx(int event_id) {
  for (int i = 0; i < count_events; i++) {
    if (events[i].id == event_id)
      return i;
  }
  fprintf(stderr, "Event ID not found: %d\n", event_id);
  return -1;
}

int get_event_owner(int event_id) {
  int idx = get_event_idx(event_id);
  if (idx == -1)
    return -1;
  return events[idx].owner;
}

char *get_event_name(int event_id) {
  int idx = get_event_idx(event_id);
  if (idx == -1)
    return NULL;
  return events[idx].name;
}

bool event_exists(int event_id) {
  return get_event_idx(event_id) != -1;
}

int db_read_events() {
  FILE *fp = fopen(DB_EVENTS, "r");
  if (fp == NULL) {
    perror("Error opening events database");
    exit(1);
  }

  char line[MAX_LINE_TEXT];
  count_events = 0;
  gen_event_id = 1;

  while (fgets(line, sizeof(line), fp)) {
    char *token;
    int i = 0;

    // Parse ID
    token = strtok(line, "|");
    if (token == NULL)
      continue;
    events[count_events].id = atoi(token);
    gen_event_id++;

    // Parse name
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_EVENT_NAME) {
      fprintf(stderr, "Event name too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(events[count_events].name, token, strlen(token) + 1);

    // Parse date
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_EVENT_DATE) {
      fprintf(stderr, "Event date too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(events[count_events].date, token, strlen(token) + 1);

    // Parse address
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_EVENT_ADDRESS) {
      fprintf(stderr, "Event address too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(events[count_events].address, token, strlen(token) + 1);

    // Parse type
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    events[count_events].type = atoi(token);

    // Parse details
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    token = trim_blank(token);
    if (strlen(token) > LEN_EVENT_DETAILS) {
      fprintf(stderr, "Event details too long: \"%s\"\n", token);
      fclose(fp);
      exit(1);
    }
    memcpy(events[count_events].details, token, strlen(token) + 1);

    // Parse owner
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    events[count_events].owner = atoi(token);

    // Parse members
    token = strtok(NULL, "|");
    if (token == NULL)
      continue;
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_MEMBERS) {
      events[count_events].members[i++] = atoi(token);
      token = skip_2blank(token);
    }

    // Parse requests
    token = strtok(NULL, "\n");
    if (token == NULL) {
      count_events++;
      continue;
    }
    i = 0;
    token = skip_blank(token);
    while (*token != '\0' && i < MAX_REQUESTS) {
      events[count_events].requests[i++] = atoi(token);
      token = skip_2blank(token);
    }
    count_events++;
  }

  fclose(fp);
#ifdef TEST_db_read_events
  printf("Read %d events\n", count_events);
  printf("ID |   Name   |    Date    |   Address   | Type |  Details  | Owner |  Members  | Requests\n");
  for (int i = 0; i < count_events; i++) {
    int j;
    printf("%2d | %8s | %10s | %11s | %4d | %9s | %5d | ", events[i].id, events[i].name,
        events[i].date, events[i].address, events[i].type, events[i].details, events[i].owner);

    for (j = 0; j < MAX_MEMBERS && events[i].members[j] != 0; j++) {
      printf("%d ", events[i].members[j]);
    }
    printf("%*s", abs(9 - 2*j), "");
    printf(" | ");

    for (j = 0; j < MAX_REQUESTS && events[i].requests[j] != 0; j++) {
      printf("%d ", events[i].requests[j]);
    }
    printf("\n");
  }
#endif
  return 0;
}

int db_save_events0() {
  FILE *fp = fopen(DB_EVENTS_BACKUP, "w");
  if (fp == NULL) {
    perror("Error opening events database");
    exit(1);
  }

  for (int i = 0; i < count_events; i++) {
    // Write ID, name, date, address, type, details, and owner
    fprintf(fp, "%d | %s | %s | %s | %d | %s | %d | ", events[i].id, events[i].name,
        events[i].date, events[i].address, events[i].type, events[i].details, events[i].owner);

    // Write members
    for (int j = 0; j < MAX_MEMBERS && events[i].members[j] != 0; j++) {
      fprintf(fp, "%d ", events[i].members[j]);
    }
    fprintf(fp, "| ");

    // Write requests
    for (int j = 0; j < MAX_REQUESTS && events[i].requests[j] != 0; j++) {
      fprintf(fp, "%d ", events[i].requests[j]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}

int db_save_events() {
  FILE *fp = fopen(DB_EVENTS_BACKUP, "w");
  if (fp == NULL) {
    perror("Error opening events database");
    exit(1);
  }

  for (int i = 0; i < count_events; i++) {
    int j;
    fprintf(fp, "%d | %8s | %10s | %11s | %4d | %9s | %5d | ", events[i].id, events[i].name,
        events[i].date, events[i].address, events[i].type, events[i].details, events[i].owner);

    for (j = 0; j < MAX_MEMBERS && events[i].members[j] != 0; j++) {
      fprintf(fp, "%d ", events[i].members[j]);
    }
    fprintf(fp, "%*s", abs(9 - 2*j), "");
    fprintf(fp, " | ");

    for (j = 0; j < MAX_REQUESTS && events[i].requests[j] != 0; j++) {
      fprintf(fp, "%d ", events[i].requests[j]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}