#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "traffic.h"
#include "data_reader.h"
#include "thread_manager.h"
#include "controller.h"
#include "display.h"
#include "logger.h"


const char* dir_names[NUM_DIRECTIONS] = { "NORTH", "EAST", "SOUTH", "WEST" };
Traffic traffic_data[NUM_DIRECTIONS] = {0};
Direction current_green = NORTH;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int simulation_running = 1;
int total_vehicles[4] = {0};
Traffic current_traffic[4] = {0};
TrafficBuffer buffer;

// load_vehicle_data implementation
int load_vehicle_data(const char *filename) {
    printf("Loading vehicle data from %s...\n", filename);
    int result = read_traffic_data(filename, &buffer);
    if (!result) {
        printf("Failed to load vehicle data.\n");
    } else {
        printf("Vehicle data loaded successfully.\n");
    }
    return result;
}

int main() {
    printf("Starting traffic simulation...\n");
    logger_init();
    printf("Traffic buffer initialized with capacity %d.\n", BUFFER_SIZE);
    traffic_buffer_init(&buffer, BUFFER_SIZE);

    printf("LOADING vehicle data from data/traffic_dataset.csv...\n");

    pthread_t producer, controller, display;
    pthread_t direction_threads[4];

    // Create producer thread
    printf("Creating producer thread...\n");
    if (pthread_create(&producer, NULL, producer_thread, NULL) != 0) {
        perror("Failed to create producer thread");
        return 1;
    }
    printf("Producer thread created.\n");

    // Create controller thread
    printf("Creating controller thread...\n");
    if (pthread_create(&controller, NULL, controller_thread, NULL) != 0) {
        perror("Failed to create controller thread");
        return 1;
    }
    printf("Controller thread created.\n");

    // Create direction threads
    for (int i = 0; i < 4; i++) {
        printf("Creating direction thread for %s...\n", dir_names[i]);
        if (pthread_create(&direction_threads[i], NULL, direction_thread, (void*)(long)i) != 0) {
            perror("Failed to create direction thread");
            return 1;
        }
        printf("Direction thread for %s created.\n", dir_names[i]);
    }

    // Create display thread
    printf("Creating display thread...\n");
    if (pthread_create(&display, NULL, display_thread, NULL) != 0) {
        perror("Failed to create display thread");
        return 1;
    }
    printf("Display thread created.\n");

    // Join threads
    printf("Joining producer thread...\n");
    pthread_join(producer, NULL);
    printf("Producer thread joined.\n");

    printf("Joining controller thread...\n");
    pthread_join(controller, NULL);
    printf("Controller thread joined.\n");

    for (int i = 0; i < 4; i++) {
        printf("Joining direction thread for %s...\n", dir_names[i]);
        pthread_join(direction_threads[i], NULL);
        printf("Direction thread for %s joined.\n", dir_names[i]);
    }

    printf("Joining display thread...\n");
    pthread_join(display, NULL);
    printf("Display thread joined.\n");

    traffic_buffer_destroy(&buffer);
    logger_destroy();
    printf("Simulation complete.\n");
    return 0;
}

// int main() {
//     printf("Starting traffic simulation...\n");
//     init_logger();
//     printf("Logger initialized.\n");

//     // Initialize the traffic buffer
//     if (traffic_buffer_init(&buffer, BUFFER_SIZE) != 0) {
//         printf("Failed to initialize traffic buffer.\n");
//         close_logger();
//         return 1;
//     }
//     printf("Traffic buffer initialized with capacity %d.\n", BUFFER_SIZE);

//     if (!load_vehicle_data("data/traffic_dataset.csv")) {
//         printf("Exiting due to failure in load_vehicle_data.\n");
//         close_logger();
//         return 1;
//     }

//     printf("Starting threads...\n");
//     if (init_threads("data/traffic_dataset.csv") != 0) {
//         printf("Failed to initialize threads.\n");
//         close_logger();
//         return 1;
//     }

//     printf("Threads started, joining threads...\n");
//     join_threads();
//     printf("Threads joined.\n");

//     traffic_buffer_destroy(&buffer);
//     printf("Traffic buffer destroyed.\n");

//     close_logger();
//     printf("Logger closed. Simulation complete.\n");
//     return 0;
// }

// const char* dir_names[NUM_DIRECTIONS] = { "NORTH", "EAST", "SOUTH", "WEST" };
// Traffic traffic_data[NUM_DIRECTIONS] = {0};
// Direction current_green = NORTH;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// int simulation_running = 1;
// int total_vehicles[4] = {0};
// Traffic current_traffic[4] = {0};
// TrafficBuffer buffer;

// // load_vehicle_data implementation
// int load_vehicle_data(const char *filename) {
//     return read_traffic_data(filename, &buffer);
// }

// int main()
// {
//     init_logger();

//     traffic_buffer_init(&buffer, BUFFER_SIZE);

//     if (!load_vehicle_data("data/traffic_dataset.csv")) {
//         printf("Failed to load vehicle data.\n");
//         close_logger();
//         return 1;
//     }

//     if (init_threads("data/traffic_dataset.csv") != 0) {
//         printf("Failed to initialize threads.\n");
//         close_logger();
//         return 1;
//     }

//     join_threads();

//     // pthread_t controller_thread;
//     // pthread_create(&controller_thread, NULL, green_light_controller, NULL);

//     // pthread_t direction_threads[NUM_DIRECTIONS];
//     // for (int i = 0; i < NUM_DIRECTIONS; i++) {
//     //     pthread_create(&direction_threads[i], NULL, direction_thread, (void*)(long)i);
//     // }

//     // pthread_join(controller_thread, NULL);

//     // for (int i = 0; i < NUM_DIRECTIONS; i++) {
//     //     pthread_join(direction_threads[i], NULL);
//     // }

//     traffic_buffer_destroy(&buffer);
//     close_logger();
//     return 0;
// }

