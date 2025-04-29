#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include <pthread.h>
#include "traffic.h"
#include "display.h"
#include "thread_manager.h"   // for init_threads(), join_threads()
#include "globals.h"

// Extern declarations to use variables from traffic.h and display.h
extern int total_vehicles[4];
extern Direction current_green;
extern pthread_mutex_t mutex;  // Extern mutex declaration
const int NUM_DIRECTIONS = 4;  // Define NUM_DIRECTIONS as a constant

// Constants
#define NUM_DIRECTIONS 4  // Define NUM_DIRECTIONS here

const char* dir_names[]       = { "NORTH", "EAST", "SOUTH", "WEST" };
//int simulation_running = 1;  // Define simulation_running here

// Initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex initialization

// Function to process traffic in a specific direction
// void process_direction(const char *direction) {
//     int remaining_vehicles = total_vehicles[current_green];

//     pthread_mutex_lock(&mutex);  // Lock mutex

//     while (remaining_vehicles > 0) {
//         if (remaining_vehicles == total_vehicles[current_green]) {
//             printf("[%s] [GREEN] 🚗 Vehicle passing... (remaining: %d)\n", direction_names[current_green], remaining_vehicles);
//         } else {
//             printf("[%s] [GREEN] 🚗 Vehicle passing... (remaining: %d)\n", direction_names[current_green], remaining_vehicles);
//         }

//         sleep(1);
//         remaining_vehicles--;
//     }

//     pthread_mutex_unlock(&mutex);  // Unlock mutex
// }

// int main() {
//     for (int i = 0; i < NUM_DIRECTIONS; i++) {
//         total_vehicles[i] = 20;
//     }

//     current_green = 0;

//     while (simulation_running) {
//         process_direction(direction_names[current_green]);
//         current_green = (current_green + 1) % NUM_DIRECTIONS;
//     }

//     return 0;
// }

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s traffic_dataset.csv\n", argv[0]);
        return 1;
    }

    // 1) Initialize your circular buffer:
    traffic_buffer_init(&buffer, BUFFER_SIZE);

    // 2) Launch producer + direction + controller threads,
    //    passing the CSV filename to the producer:
    if (init_threads(argv[1]) != 0) {
        fprintf(stderr, "ERROR: failed to init threads\n");
        return 1;
    }

    // 3) Main thread can optionally render the table continuously:
    while (simulation_running) {
        print_table();
        sleep(1);
    }

    // 4) Clean shutdown: wait for all threads to finish
    join_threads();
    traffic_buffer_destroy(&buffer);

    return 0;
}

    

