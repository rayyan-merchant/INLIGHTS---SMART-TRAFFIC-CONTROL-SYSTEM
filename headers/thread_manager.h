#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "traffic.h" // Ensure that traffic.h is included for the necessary definitions

#include <pthread.h>
extern pthread_mutex_t mutex;


int init_threads(char* filename);
void join_threads();

#endif

