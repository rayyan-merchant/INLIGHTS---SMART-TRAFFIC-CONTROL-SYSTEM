#include "controller.h"
#include "traffic.h"
#include "utils.h"
#include "thread_manager.h"
#include "globals.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Thread IDs
static pthread_t producer_tid;
static pthread_t direction_tids[4];
static pthread_t controller_tid;

void* producer_thread(void* arg) {
    char* filename = (char*)arg;
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        IntersectionData data;
        if (parse_line(line, &data) == 0) {
            traffic_buffer_enqueue(&buffer, data);
        }
    }
    fclose(fp);
    return NULL;
}

void* direction_thread(void* arg) {
    Direction d = (Direction)(long)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (!simulation_running) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        if (current_green == d && total_vehicles[d] > 0) {
            total_vehicles[d]--;
            printf("Vehicle passed from %s\n", dir_names[d]);
        }
        
        pthread_mutex_unlock(&mutex);
        sleep(100000);  // Wait a bit before checking again
    }
    return NULL;
}

int init_threads(char* filename) {
    if (pthread_create(&producer_tid, NULL, producer_thread, (void*)filename) != 0) {
        return -1;
    }
    for (int d = 0; d < 4; d++) {
        if (pthread_create(&direction_tids[d], NULL, direction_thread, (void*)(long)d) != 0) {
            return -1;  // Return error if thread creation fails
        }
    }
    if (pthread_create(&controller_tid, NULL, controller_thread, NULL) != 0) {
        return -1;
    }
    return 0;
}

void join_threads() {
    pthread_join(producer_tid, NULL);
    pthread_join(controller_tid, NULL);
    for (int d = 0; d < 4; d++) {
        pthread_cancel(direction_tids[d]);
        pthread_join(direction_tids[d], NULL);
    }
}

