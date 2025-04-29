#ifndef DISPLAY_H
#define DISPLAY_H

void* direction_thread(void* arg);
void* green_light_controller(void* arg);
void print_table();
extern const int NUM_DIRECTIONS;  // Declare NUM_DIRECTIONS as extern

#endif

