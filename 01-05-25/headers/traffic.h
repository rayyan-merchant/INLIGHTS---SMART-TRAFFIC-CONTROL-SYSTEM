#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <pthread.h>

#define BUFFER_SIZE 1440  // Define buffer size for TrafficBuffer

typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    int normal_vehicles;
    int emergency_vehicles;
} Traffic;

// Intersection data structure
typedef struct {
    int minute_of_day;
    Traffic dir[4];  // Traffic data for NORTH, SOUTH, EAST, WEST
} IntersectionData;

typedef struct {
    IntersectionData* buffer;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} TrafficBuffer;

#define NUM_DIRECTIONS 4

extern const char* dir_names[NUM_DIRECTIONS];
extern Traffic traffic_data[NUM_DIRECTIONS];
extern Direction current_green;
extern pthread_mutex_t mutex;
extern int simulation_running;
extern int total_vehicles[4];
extern Traffic current_traffic[4];
extern TrafficBuffer buffer;

int load_vehicle_data(const char *filename);
int traffic_buffer_init(TrafficBuffer* buffer, size_t capacity);
int traffic_buffer_enqueue(TrafficBuffer* buffer, IntersectionData item);
int traffic_buffer_dequeue(TrafficBuffer* buffer, IntersectionData* out);
void traffic_buffer_destroy(TrafficBuffer* buffer);

#endif