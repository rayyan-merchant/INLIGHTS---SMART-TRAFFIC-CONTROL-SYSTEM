#ifndef LOGGER_H
#define LOGGER_H

void init_logger();
void close_logger();
void log_event(const char* fmt, ...);

#endif

