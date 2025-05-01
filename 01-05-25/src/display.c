#define _POSIX_C_SOURCE 199309L  // Enable nanosleep() in strict C mode
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>  // Ensure this is included for nanosleep
#include "traffic.h"
#include "display.h"

// Function to sleep for half a second using nanosleep
void sleep_half_second() {
    struct timespec req;
    req.tv_sec = 0;               // Seconds part of the sleep
    req.tv_nsec = 500000000L;     // 500 milliseconds = 0.5 seconds

    int result = nanosleep(&req, NULL); // Sleep for the given time
    if (result != 0) {
        perror("nanosleep failed");
    }
}

// void* direction_thread(void* arg) {
//     int direction = (int)(long)arg;
//     while (simulation_running) {
//         pthread_mutex_lock(&mutex);
//         if (current_green == direction) {
//             if (traffic_data[direction].emergency_vehicles > 0) {
//                 traffic_data[direction].emergency_vehicles--;
//                 printf("[Emergency Vehicle] passed from %s 🚑\n", dir_names[direction]);
//             } else if (traffic_data[direction].normal_vehicles > 0) {
//                 traffic_data[direction].normal_vehicles--;
//                 printf("[Normal Vehicle] passed from %s 🚗\n", dir_names[direction]);
//             }
//         }

//         // Check if all vehicles are processed
//         int all_vehicles_processed = 1;  // Assume true unless proven otherwise
//         for (int i = 0; i < NUM_DIRECTIONS; i++) {
//             if (traffic_data[i].normal_vehicles > 0 || traffic_data[i].emergency_vehicles > 0) {
//                 all_vehicles_processed = 0;
//                 break;
//             }
//         }
//         if (all_vehicles_processed) {
//             simulation_running = 0;  // Signal all threads to stop
//         }

//         pthread_mutex_unlock(&mutex);

//         sleep_half_second();  // Call the sleep function
//     }
//     return NULL;
// }

// void* green_light_controller(void* arg) {
//     while (simulation_running) {
//         pthread_mutex_lock(&mutex);
//         current_green = (current_green + 1) % NUM_DIRECTIONS;
//         pthread_mutex_unlock(&mutex);

//         print_table();
//         sleep_half_second();  // Call the sleep function
//     }
//     return NULL;
// }

void* display_thread(void* arg) {
    printf("Display thread started.\n");
    while (simulation_running) {
        pthread_mutex_lock(&mutex);
        print_table();
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
    printf("Display thread: Simulation stopped, exiting.\n");
    return NULL;
}


void print_table() {
    printf("\n🚦 Smart Traffic Simulation 🚦\n");
    printf("+-----------------+------------------+------------------+\n");
    printf("| Direction       | Normal Vehicles   | Emergency Vehicles |\n");
    printf("+-----------------+------------------+------------------+\n");

    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        printf("| %-15s | %-16d | %-16d |\n",
            dir_names[i],
            traffic_data[i].normal_vehicles,
            traffic_data[i].emergency_vehicles);
    }

    printf("+-----------------+------------------+------------------+\n");
    printf("\n[Green Light]: %s 🚦\n", dir_names[current_green]);
}

