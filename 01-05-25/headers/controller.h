#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <pthread.h>
#include "traffic.h" // Ensure that traffic.h is included for the necessary definitions

void* controller_thread(void* arg);

#endif

