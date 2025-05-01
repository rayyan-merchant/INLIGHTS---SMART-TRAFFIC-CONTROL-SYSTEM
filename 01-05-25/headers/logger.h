#ifndef LOGGER_H
#define LOGGER_H

void logger_init();
void logger_destroy();
void log_event(const char* fmt, ...);

#endif