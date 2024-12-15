#ifndef LOGGER_H
#define LOGGER_H

void init_logger(const char* log_file);
void log_info(const char* message);
void log_error(const char* message);
void close_logger();

#endif