#include "controller.h"
#include "traffic.h"
#include "logger.h"
#include "globals.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

extern TrafficBuffer buffer;
extern Traffic current_traffic[4];
extern int total_vehicles[4];
extern Direction current_green;
extern pthread_mutex_t mutex;

void* controller_thread(void* arg) {
    while (1) {
        IntersectionData data;
        if (traffic_buffer_dequeue(&buffer, &data) != 0) {
            break;
        }
        if (data.minute_of_day == -1) {
            break;
        }

        pthread_mutex_lock(&mutex);
        for (int d = 0; d < 4; d++) {
            current_traffic[d].cars += data.dir[d].cars;
            current_traffic[d].bikes += data.dir[d].bikes;
            current_traffic[d].trucks += data.dir[d].trucks;
            current_traffic[d].emergency += data.dir[d].emergency;
            total_vehicles[d] = current_traffic[d].cars + current_traffic[d].bikes + 
                               current_traffic[d].trucks + current_traffic[d].emergency;
        }

        double max_load = -1;
        Direction max_dir;
        for (int d = 0; d < 4; d++) {
            double load = calc_load(current_traffic[d]);
            if (load > max_load) {
                max_load = load;
                max_dir = (Direction)d;
            }
        }

        if (current_green != max_dir) {
            current_green = max_dir;
        }

        int duration = 5 + (int)(total_vehicles[current_green] * 0.2 + 0.5);
        if (duration > 30) duration = 30;
        if (duration < 5) duration = 5;
        
        pthread_mutex_unlock(&mutex);
        
        log_event("Green light for %s for %d seconds", dir_names[current_green], duration);
        sleep(duration);
    }
    return NULL;
}

