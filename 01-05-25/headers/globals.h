// headers/globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>      // for pthread_mutex_t
#include "traffic.h"      // for the Direction enum

// The one true definition of these globals lives in main.c:
extern pthread_mutex_t mutex;
extern int total_vehicles[];           // array size is defined in main.c
extern const char *dir_names[4];  // likewise defined in main.c

#endif // GLOBALS_H
