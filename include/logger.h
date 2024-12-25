#ifndef LOGGER_H
#define LOGGER_H

#define DB_LOG "log.txt"

int log_activity(char* ip, int port, char* username, char* command, char* status);

#endif