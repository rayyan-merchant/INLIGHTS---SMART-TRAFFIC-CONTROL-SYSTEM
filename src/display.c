#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include <pthread.h>  // Include pthread.h for mutex support
#include "traffic.h"

// Extern declarations to use variables from main.c
extern int total_vehicles[4];
extern Direction current_green;
extern pthread_mutex_t mutex;
extern int simulation_running;  // Declare extern to access simulation_running
extern const char* dir_names[];  // Declare extern to access dir_names
extern int NUM_DIRECTIONS;  // Declare NUM_DIRECTIONS as extern

// ANSI escape codes for colors
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"

// Function to print the live traffic table
void print_table() {
    system("clear");  // Clear the console

    printf("\n🚦 Smart Traffic Simulation 🚦\n");
    printf("+-----------------+------------------+\n");
    printf("| Direction       | Cars Passed      |\n");
    printf("+-----------------+------------------+\n");

    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (i == current_green) {
            printf("| %-15s | %-16d | %s<-- GREEN%s\n", dir_names[i], total_vehicles[i], COLOR_GREEN, COLOR_RESET);
        } else {
            printf("| %-15s | %-16d |\n", dir_names[i], total_vehicles[i]);
        }
    }

    printf("+-----------------+------------------+\n");
    printf("\n[Green Light]: %s%s%s 🚗💨\n", COLOR_GREEN, dir_names[current_green], COLOR_RESET);
}

void display_vehicle_pass(const char *direction, const char *vehicle_type, int remaining) {
    printf("[%s] [%s] %s vehicle passing... (remaining: %d)\n", direction, vehicle_type, vehicle_type, remaining);
}

void simulate_vehicle_pass(const char *direction, const char *vehicle_type, int *remaining, int emergency) {
    int delay = (emergency) ? 1 : 1;  // 1 second for emergency or normal vehicle

    while (*remaining > 0) {
        display_vehicle_pass(direction, vehicle_type, *remaining);
        sleep(delay);  // Delay for 1 second
        (*remaining)--;  // Decrease remaining vehicles
    }
}

