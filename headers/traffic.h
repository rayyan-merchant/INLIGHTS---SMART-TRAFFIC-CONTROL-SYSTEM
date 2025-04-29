#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <pthread.h>
#include <stddef.h>  // For size_t

// Define directions
typedef enum {
    NORTH = 0,
    SOUTH = 1,
    EAST  = 2,
    WEST  = 3
} Direction;

// Traffic structure
typedef struct {
    int cars;
    int bikes;
    int trucks;
    int emergency;
} Traffic;

// Intersection data structure
typedef struct {
    Direction direction;
    int vehicle_count;
    int minute_of_day;
    Traffic dir[4];
} IntersectionData;

// Traffic buffer structure
typedef struct {
    IntersectionData* buffer;
    size_t capacity;
    size_t size;
    size_t front;
    size_t rear;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} TrafficBuffer;

// Global declarations
extern TrafficBuffer buffer;
extern Traffic current_traffic[4];
extern Direction current_green;
extern int simulation_running;

// Functions
double calc_load(Traffic t);
int traffic_buffer_init(TrafficBuffer* buffer, size_t capacity);
int traffic_buffer_enqueue(TrafficBuffer* buffer, IntersectionData item);
int traffic_buffer_dequeue(TrafficBuffer* buffer, IntersectionData* out);
void traffic_buffer_destroy(TrafficBuffer* buffer);

// Macros
#define BUFFER_SIZE 100  // Set buffer size

#endif // TRAFFIC_H

