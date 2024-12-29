#include "event.h"
#include "session.h"

// Global variables
event_t events[MAX_EVENTS] = {0};
int count_events = 0;
int gen_event_id = 1;

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
    token = trim_space(token);
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
    token = trim_space(token);
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
    token = trim_space(token);
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
    token = trim_space(token);
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
    token = skip_space(token);
    while (*token != '\0' && i < MAX_MEMBERS) {
      events[count_events].members[i++] = atoi(token);
      token = skip_2space(token);
    }

    // Parse requests
    token = strtok(NULL, "\n");
    if (token == NULL) {
      count_events++;
      continue;
    }
    i = 0;
    token = skip_space(token);
    while (*token != '\0' && i < MAX_REQUESTS) {
      events[count_events].requests[i++] = atoi(token);
      token = skip_2space(token);
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