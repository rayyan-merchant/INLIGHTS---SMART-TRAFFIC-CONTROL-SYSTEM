#include "controller.h"
#include "traffic.h"
#include "logger.h"
#include "utils.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

extern TrafficBuffer buffer;
extern Traffic current_traffic[4];
extern int total_vehicles[4];
extern Direction current_green;
extern pthread_mutex_t mutex;

void* controller_thread(void* arg) {
    printf("Controller thread started.\n");
    while (1) {
        IntersectionData data;
        printf("Controller: Attempting to dequeue data...\n");
        if (traffic_buffer_dequeue(&buffer, &data) != 0) {
            printf("Controller: Failed to dequeue data, buffer empty.\n");
            break;
        }
        if (data.minute_of_day == -1) {
            printf("Controller: Dequeued sentinel, exiting.\n");
            break;
        }
        printf("Controller: Dequeued data for minute %d.\n", data.minute_of_day);

        pthread_mutex_lock(&mutex);
        for (int d = 0; d < 4; d++) {
            current_traffic[d].normal_vehicles += data.dir[d].normal_vehicles;
            current_traffic[d].emergency_vehicles += data.dir[d].emergency_vehicles;
            total_vehicles[d] = current_traffic[d].normal_vehicles + current_traffic[d].emergency_vehicles;
            printf("Controller: Direction %s - Normal: %d, Emergency: %d, Total: %d\n",
                   dir_names[d], current_traffic[d].normal_vehicles,
                   current_traffic[d].emergency_vehicles, total_vehicles[d]);
        }

        double max_load = -1;
        Direction max_dir;
        for (int d = 0; d < 4; d++) {
            Traffic t = current_traffic[d];
            double load = calc_load(t);
            if (load > max_load) {
                max_load = load;
                max_dir = (Direction)d;
            }
        }

        if (current_green != max_dir) {
            current_green = max_dir;
            printf("Controller: Green light changed to %s.\n", dir_names[current_green]);
        }

        int duration = 5 + (int)(total_vehicles[current_green] * 0.2 + 0.5);
        if (duration > 30) duration = 30;
        if (duration < 5) duration = 5;

        pthread_mutex_unlock(&mutex);

        log_event("Green light for %s for %d seconds", dir_names[current_green], duration);
        printf("Controller: Green light for %s for %d seconds.\n", dir_names[current_green], duration);
        // sleep(duration); // Comment out for faster simulation
        // printf("Controller: Sleeping for %d seconds to stretch to 1 minute.\n", 60 - duration);
        // sleep(60 - duration); // Comment out for faster simulation
    }

    pthread_mutex_lock(&mutex);
    simulation_running = 0;
    printf("Controller: Simulation complete, setting simulation_running to 0.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// void* controller_thread(void* arg) {
//     printf("Controller thread started.\n");
//     while (1) {
//         IntersectionData data;
//         printf("Controller: Attempting to dequeue data...\n");
//         if (traffic_buffer_dequeue(&buffer, &data) != 0) {
//             printf("Controller: Failed to dequeue data, buffer empty.\n");
//             break;
//         }
//         if (data.minute_of_day == -1) {
//             printf("Controller: Dequeued sentinel, exiting.\n");
//             break;
//         }
//         printf("Controller: Dequeued data for minute %d.\n", data.minute_of_day);

//         pthread_mutex_lock(&mutex);
//         for (int d = 0; d < 4; d++) {
//             current_traffic[d].normal_vehicles += data.dir[d].normal_vehicles;
//             current_traffic[d].emergency_vehicles += data.dir[d].emergency_vehicles;
//             total_vehicles[d] = current_traffic[d].normal_vehicles + current_traffic[d].emergency_vehicles;
//             printf("Controller: Direction %s - Normal: %d, Emergency: %d, Total: %d\n",
//                    dir_names[d], current_traffic[d].normal_vehicles,
//                    current_traffic[d].emergency_vehicles, total_vehicles[d]);
//         }

//         double max_load = -1;
//         Direction max_dir;
//         for (int d = 0; d < 4; d++) {
//             Traffic t = current_traffic[d];
//             double load = calc_load(t);
//             if (load > max_load) {
//                 max_load = load;
//                 max_dir = (Direction)d;
//             }
//         }

//         if (current_green != max_dir) {
//             current_green = max_dir;
//             printf("Controller: Green light changed to %s.\n", dir_names[current_green]);
//         }

//         int duration = 5 + (int)(total_vehicles[current_green] * 0.2 + 0.5);
//         if (duration > 30) duration = 30;
//         if (duration < 5) duration = 5;
        
//         pthread_mutex_unlock(&mutex);
        
//         log_event("Green light for %s for %d seconds", dir_names[current_green], duration);
//         printf("Controller: Green light for %s for %d seconds.\n", dir_names[current_green], duration);
//         sleep(duration);
//         printf("Controller: Sleeping for %d seconds to stretch to 1 minute.\n", 60 - duration);
//         sleep(60 - duration);
//     }

//     pthread_mutex_lock(&mutex);
//     simulation_running = 0;
//     printf("Controller: Simulation complete, setting simulation_running to 0.\n");
//     pthread_mutex_unlock(&mutex);
//     return NULL;
// }

// void* controller_thread(void* arg) {
//     printf("Controller thread started.\n");
//     int rows_processed = 0;
//     int target_rows = 2;

//     while (rows_processed < target_rows) {
//         IntersectionData data;
//         printf("Controller: Attempting to dequeue data (row %d)...\n", rows_processed + 1);
//         if (traffic_buffer_dequeue(&buffer, &data) != 0) {
//             printf("Controller: Failed to dequeue data, buffer empty.\n");
//             break;
//         }
//         if (data.minute_of_day == -1) {
//             printf("Controller: Dequeued sentinel, exiting.\n");
//             break;
//         }
//         printf("Controller: Dequeued data for minute %d.\n", data.minute_of_day);

//         pthread_mutex_lock(&mutex);
//         for (int d = 0; d < 4; d++) {
//             current_traffic[d].normal_vehicles += data.dir[d].normal_vehicles;
//             current_traffic[d].emergency_vehicles += data.dir[d].emergency_vehicles;
//             total_vehicles[d] = current_traffic[d].normal_vehicles + current_traffic[d].emergency_vehicles;
//             printf("Controller: Direction %s - Normal: %d, Emergency: %d, Total: %d\n",
//                    dir_names[d], current_traffic[d].normal_vehicles,
//                    current_traffic[d].emergency_vehicles, total_vehicles[d]);
//         }

//         double max_load = -1;
//         Direction max_dir;
//         for (int d = 0; d < 4; d++) {
//             Traffic t = current_traffic[d];
//             double load = calc_load(t);
//             if (load > max_load) {
//                 max_load = load;
//                 max_dir = (Direction)d;
//             }
//         }

//         if (current_green != max_dir) {
//             current_green = max_dir;
//             printf("Controller: Green light changed to %s.\n", dir_names[current_green]);
//         }

//         int duration = 5 + (int)(total_vehicles[current_green] * 0.2 + 0.5);
//         if (duration > 30) duration = 30;
//         if (duration < 5) duration = 5;
        
//         pthread_mutex_unlock(&mutex);
        
//         log_event("Green light for %s for %d seconds", dir_names[current_green], duration);
//         printf("Controller: Green light for %s for %d seconds.\n", dir_names[current_green], duration);
//         sleep(duration);

//         rows_processed++;
//         printf("Controller: Processed row %d, sleeping for %d seconds to stretch to 1 minute.\n", rows_processed, 60 - duration);
//         sleep(60 - duration);
//     }

//     pthread_mutex_lock(&mutex);
//     simulation_running = 0;
//     printf("Controller: Simulation complete, setting simulation_running to 0.\n");
//     pthread_mutex_unlock(&mutex);
//     return NULL;
// }

// #include "controller.h"
// #include "traffic.h"
// #include "logger.h"
// #include <unistd.h>
// #include <pthread.h>
// #include <stdio.h>

// extern TrafficBuffer buffer;
// extern Traffic current_traffic[4];
// extern int total_vehicles[4];
// extern Direction current_green;
// extern pthread_mutex_t mutex;

// void* controller_thread(void* arg) {
//     while (1) {
//         IntersectionData data;
//         if (traffic_buffer_dequeue(&buffer, &data) != 0) {
//             break;
//         }
//         if (data.minute_of_day == -1) {
//             break;
//         }

//         pthread_mutex_lock(&mutex);
//         for (int d = 0; d < 4; d++) {
//             current_traffic[d].cars += data.dir[d].cars;
//             current_traffic[d].bikes += data.dir[d].bikes;
//             current_traffic[d].trucks += data.dir[d].trucks;
//             current_traffic[d].emergency += data.dir[d].emergency;
//             total_vehicles[d] = current_traffic[d].cars + current_traffic[d].bikes + 
//                                current_traffic[d].trucks + current_traffic[d].emergency;
//         }

//         double max_load = -1;
//         Direction max_dir;
//         for (int d = 0; d < 4; d++) {
//             double load = calc_load(current_traffic[d]);
//             if (load > max_load) {
//                 max_load = load;
//                 max_dir = (Direction)d;
//             }
//         }

//         if (current_green != max_dir) {
//             current_green = max_dir;
//         }

//         int duration = 5 + (int)(total_vehicles[current_green] * 0.2 + 0.5);
//         if (duration > 30) duration = 30;
//         if (duration < 5) duration = 5;
        
//         pthread_mutex_unlock(&mutex);
        
//         log_event("Green light for %s for %d seconds", dir_names[current_green], duration);
//         sleep(duration);
//     }
//     return NULL;
// }

