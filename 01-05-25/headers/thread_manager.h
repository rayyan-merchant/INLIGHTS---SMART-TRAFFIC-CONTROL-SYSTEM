#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H
#include <pthread.h>
#include "traffic.h" // Ensure that traffic.h is included for the necessary definitions

int init_threads(char* filename);
void* producer_thread(void* arg);
void* direction_thread(void* arg);
void join_threads();

#endif

