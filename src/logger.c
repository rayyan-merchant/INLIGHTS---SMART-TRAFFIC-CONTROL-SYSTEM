#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* log_file = NULL;

void init_logger() {
    log_file = fopen("simulation.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
    }
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_event(const char* fmt, ...) {
    pthread_mutex_lock(&log_mutex);
    
    // Get current time
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Print to console with color
    printf("[%s] ", time_str);
    printf("\033[0;32m");  // Green color
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\033[0m\n");   // Reset color
    fflush(stdout);
    
    // Write to file if open
    if (log_file) {
        fprintf(log_file, "[%s] ", time_str);
        va_start(args, fmt);
        vfprintf(log_file, fmt, args);
        va_end(args);
        fprintf(log_file, "\n");
        fflush(log_file);
    }
    
    pthread_mutex_unlock(&log_mutex);
}