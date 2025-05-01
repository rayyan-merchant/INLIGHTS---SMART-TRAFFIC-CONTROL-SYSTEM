#define _POSIX_C_SOURCE 199309L
#include "controller.h"
#include "traffic.h"
#include "utils.h"
#include "thread_manager.h"
#include "globals.h"
#include "display.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Thread IDs
static pthread_t producer_tid;
static pthread_t direction_tids[4];
static pthread_t controller_tid;
static pthread_t display_tid;


void* producer_thread(void* arg) {
    printf("Producer thread started.\n");

    printf("Producer: Opening file data/traffic_dataset.csv...\n");
    FILE* file = fopen("data/traffic_dataset.csv", "r");
    if (!file) {
        perror("Producer: Failed to open traffic_dataset.csv");
        return NULL;
    }
    printf("Producer: File opened successfully.\n");

    char line[256];
    printf("Producer: Reading header line...\n");
    if (!fgets(line, sizeof(line), file)) {
        printf("Producer: Failed to read header line.\n");
        fclose(file);
        return NULL;
    }
    printf("Producer: Header line read: %s", line);

    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        row++;
        printf("Producer: Reading row %d: %s", row, line);
        IntersectionData data;
        parse_line(line, &data);
        printf("Producer: Parsed data for minute %d.\n", data.minute_of_day);
        printf("Producer: Enqueuing data for minute %d...\n", data.minute_of_day);
        traffic_buffer_enqueue(&buffer, data);
        printf("Producer: Enqueued data for minute %d.\n", data.minute_of_day);
    }

    printf("Producer: Finished reading file, enqueuing sentinel...\n");
    IntersectionData sentinel = {.minute_of_day = -1};
    traffic_buffer_enqueue(&buffer, sentinel);
    printf("Producer: Enqueued sentinel.\n");

    printf("Producer: Closing file...\n");
    fclose(file);
    printf("Producer: File closed.\n");
    return NULL;
}

// void* producer_thread(void* arg) {
//     char* filename = (char*)arg;
//     printf("Producer thread started, reading from %s...\n", filename);
//     FILE* fp = fopen(filename, "r");
//     if (!fp) {
//         perror("Producer: Failed to open CSV file");
//         simulation_running = 0;
//         return NULL;
//     }
//     char line[256];
//     if (!fgets(line, sizeof(line), fp)) {
//         printf("Producer: Failed to read header from CSV or file is empty.\n");
//         fclose(fp);
//         simulation_running = 0;
//         return NULL;
//     }
//     int lines_read = 0;
//     while (fgets(line, sizeof(line), fp)) {
//         IntersectionData data;
//         if (parse_line(line, &data) == 0) {
//             if (!simulation_running) {
//                 printf("Producer: Simulation stopped at line %d.\n", lines_read + 2);
//                 fclose(fp);
//                 return NULL;
//             }
//             traffic_buffer_enqueue(&buffer, data);  // Blocking call
//             lines_read++;
//             printf("Producer: Enqueued line %d.\n", lines_read);
//         } else {
//             printf("Producer: Failed to parse line %d: %s", lines_read + 2, line);
//         }
//     }
//     printf("Producer: Read %d lines from CSV.\n", lines_read);
//     IntersectionData sentinel = { .minute_of_day = -1 };
//     if (simulation_running) {
//         traffic_buffer_enqueue(&buffer, sentinel);  // Blocking call
//         printf("Producer: Enqueued sentinel, exiting.\n");
//     }
//     fclose(fp);
//     return NULL;
// }

void* direction_thread(void* arg) {
    Direction d = (Direction)(long)arg;
    printf("Direction thread for %s started.\n", dir_names[d]);
    while (1) {
        pthread_mutex_lock(&mutex);
        if (simulation_running && current_green == d) {
            if (current_traffic[d].emergency_vehicles > 0) {
                current_traffic[d].emergency_vehicles--;
                total_vehicles[d]--;
                printf("[Emergency Vehicle] passed from %s 🚑\n", dir_names[d]);
            } else if (current_traffic[d].normal_vehicles > 0) {
                current_traffic[d].normal_vehicles--;
                total_vehicles[d]--;
                printf("[Normal Vehicle] passed from %s 🚗\n", dir_names[d]);
            }
        } else if (!simulation_running && total_vehicles[d] > 0) {
            if (current_traffic[d].emergency_vehicles > 0) {
                current_traffic[d].emergency_vehicles--;
                total_vehicles[d]--;
                printf("[Emergency Vehicle] passed from %s 🚑 (post-simulation)\n", dir_names[d]);
            } else if (current_traffic[d].normal_vehicles > 0) {
                current_traffic[d].normal_vehicles--;
                total_vehicles[d]--;
                printf("[Normal Vehicle] passed from %s 🚗 (post-simulation)\n", dir_names[d]);
            }
        } else if (!simulation_running && total_vehicles[d] == 0) {
            pthread_mutex_unlock(&mutex);
            printf("Direction thread for %s: No vehicles left, exiting.\n", dir_names[d]);
            break;
        }
        pthread_mutex_unlock(&mutex);
        //usleep(100000); // 0.1 seconds
        struct timespec ts = {0, 100000000}; // 0.1 seconds (100 million nanoseconds)
        nanosleep(&ts, NULL);
    }
    return NULL;
}

// void* direction_thread(void* arg) {
//     Direction d = (Direction)(long)arg;
//     printf("Direction thread for %s started.\n", dir_names[d]);
//     while (simulation_running) {
//         pthread_mutex_lock(&mutex);
//         if (!simulation_running) {
//             pthread_mutex_unlock(&mutex);
//             printf("Direction thread for %s: Simulation stopped, exiting.\n", dir_names[d]);
//             break;
//         }
        
//         if (current_green == d) {
//             if (current_traffic[d].emergency_vehicles > 0) {
//                 current_traffic[d].emergency_vehicles--;
//                 total_vehicles[d]--;
//                 printf("[Emergency Vehicle] passed from %s 🚑\n", dir_names[d]);
//             } else if (current_traffic[d].normal_vehicles > 0) {
//                 current_traffic[d].normal_vehicles--;
//                 total_vehicles[d]--;
//                 printf("[Normal Vehicle] passed from %s 🚗\n", dir_names[d]);
//             }
//         }
        
//         int all_vehicles_processed = 1;
//         for (int i = 0; i < NUM_DIRECTIONS; i++) {
//             if (total_vehicles[i] > 0) {
//                 all_vehicles_processed = 0;
//                 break;
//             }
//         }
        
//         // Lock the buffer mutex to safely access buffer.size
//         size_t buffer_size;
//         pthread_mutex_lock(&buffer.mutex);
//         buffer_size = buffer.size;
//         pthread_mutex_unlock(&buffer.mutex);
        
//         if (all_vehicles_processed && buffer_size == 0) {
//             simulation_running = 0;
//             printf("Direction thread for %s: All vehicles processed and buffer empty, stopping simulation.\n", dir_names[d]);
//         }

//         pthread_mutex_unlock(&mutex);
//         sleep(100000);
//     }
//     return NULL;
// }

int init_threads(char* filename) {
    printf("init_threads: Creating producer thread...\n");
    if (pthread_create(&producer_tid, NULL, producer_thread, (void*)filename) != 0) {
        printf("init_threads: Failed to create producer thread.\n");
        return -1;
    }
    for (int d = 0; d < 4; d++) {
        printf("init_threads: Creating direction thread for %s...\n", dir_names[d]);
        if (pthread_create(&direction_tids[d], NULL, direction_thread, (void*)(long)d) != 0) {
            printf("init_threads: Failed to create direction thread for %s.\n", dir_names[d]);
            return -1;
        }
    }
    printf("init_threads: Creating controller thread...\n");
    if (pthread_create(&controller_tid, NULL, controller_thread, NULL) != 0) {
        printf("init_threads: Failed to create controller thread.\n");
        return -1;
    }
    printf("init_threads: Creating display thread...\n");
    if (pthread_create(&display_tid, NULL, display_thread, NULL) != 0) {
        printf("init_threads: Failed to create display thread.\n");
        return -1;
    }
    printf("init_threads: All threads created successfully.\n");
    return 0;
}

void join_threads() {
    printf("join_threads: Joining producer thread...\n");
    pthread_join(producer_tid, NULL);
    printf("join_threads: Joining controller thread...\n");
    pthread_join(controller_tid, NULL);
    printf("join_threads: Joining display thread...\n");
    pthread_join(display_tid, NULL);
    for (int d = 0; d < 4; d++) {
        printf("join_threads: Canceling and joining direction thread for %s...\n", dir_names[d]);
        pthread_cancel(direction_tids[d]);
        pthread_join(direction_tids[d], NULL);
    }
    printf("join_threads: All threads joined.\n");
}

// // Thread IDs
// static pthread_t producer_tid;
// static pthread_t direction_tids[4];
// static pthread_t controller_tid;

// void* producer_thread(void* arg) {
//     char* filename = (char*)arg;
//     FILE* fp = fopen(filename, "r");
//     if (!fp) {
//         perror("fopen");
//         return NULL;
//     }
//     char line[256];
//     fgets(line, sizeof(line), fp);  //skip header
//     while (fgets(line, sizeof(line), fp)) {
//         IntersectionData data;
//         if (parse_line(line, &data) == 0) {
//             traffic_buffer_enqueue(&buffer, data);
//         }
//     }
//     IntersectionData sentinel = { .minute_of_day = -1 };
//     traffic_buffer_enqueue(&buffer, sentinel);
//     fclose(fp);
//     return NULL;
// }

// void* direction_thread(void* arg) {
//     Direction d = (Direction)(long)arg;
//     while (simulation_running) {
//         pthread_mutex_lock(&mutex);
//         if (!simulation_running) {
//             pthread_mutex_unlock(&mutex);
//             break;
//         }
        
//         if (current_green == d && total_vehicles[d] > 0) {
//             total_vehicles[d]--;
//             printf("Vehicle passed from %s\n", dir_names[d]);
//         }
        
//         // Check if all vehicles are processed and no more data in buffer
//         int all_vehicles_processed = 1;
//         for (int i = 0; i < NUM_DIRECTIONS; i++) {
//             if (total_vehicles[i] > 0) {
//                 all_vehicles_processed = 0;
//                 break;
//             }
//         }
//         if (all_vehicles_processed && buffer.size == 0) {
//             simulation_running = 0;
//         }

//         pthread_mutex_unlock(&mutex);
//         sleep(100000);  // Wait a bit before checking again
//     }
//     return NULL;
// }

// int init_threads(char* filename) {
//     if (pthread_create(&producer_tid, NULL, producer_thread, (void*)filename) != 0) {
//         return -1;
//     }
//     for (int d = 0; d < 4; d++) {
//         if (pthread_create(&direction_tids[d], NULL, direction_thread, (void*)(long)d) != 0) {
//             return -1;  // Return error if thread creation fails
//         }
//     }
//     if (pthread_create(&controller_tid, NULL, controller_thread, NULL) != 0) {
//         return -1;
//     }
//     return 0;
// }

// void join_threads() {
//     pthread_join(producer_tid, NULL);
//     pthread_join(controller_tid, NULL);
//     for (int d = 0; d < 4; d++) {
//         pthread_cancel(direction_tids[d]);
//         pthread_join(direction_tids[d], NULL);
//     }
// }

