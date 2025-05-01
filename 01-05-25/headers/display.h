#ifndef DISPLAY_H
#define DISPLAY_H
#include "traffic.h"

void* display_thread(void* arg);
void* direction_thread(void* arg);
void* green_light_controller(void* arg);
void print_table();
void simulate_vehicle_pass(const char* direction, const char* vehicle_type, int* remaining, int emergency);

#endif

