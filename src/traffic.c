// traffic.c
#include "traffic.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h> 

// ————————————————————————————————————————————————————————————————
// 1) Definitions of the shared globals (satisfy the externs):
// ————————————————————————————————————————————————————————————————

TrafficBuffer buffer;                // the circular packet buffer
Traffic        current_traffic[4];   // aggregated traffic per direction
int            total_vehicles[4];    // sum of cars/bikes/trucks/emg
Direction      current_green = NORTH;
int            simulation_running = 1;

// ————————————————————————————————————————————————————————————————
// 2) Circular‐buffer implementation
// ————————————————————————————————————————————————————————————————

int traffic_buffer_init(TrafficBuffer* buf, size_t capacity) {
    buf->buffer   = malloc(sizeof(IntersectionData) * capacity);
    buf->capacity = capacity;
    buf->size     = 0;
    buf->front    = 0;
    buf->rear     = 0;
    pthread_mutex_init(&buf->lock, NULL);
    pthread_cond_init(&buf->not_empty, NULL);
    pthread_cond_init(&buf->not_full, NULL);
    return 0;
}

int traffic_buffer_enqueue(TrafficBuffer* buf, IntersectionData item) {
    pthread_mutex_lock(&buf->lock);
    while (buf->size == buf->capacity) {
        // wait for space
        pthread_cond_wait(&buf->not_full, &buf->lock);
    }
    buf->buffer[buf->rear] = item;
    buf->rear = (buf->rear + 1) % buf->capacity;
    buf->size++;
    pthread_cond_signal(&buf->not_empty);
    pthread_mutex_unlock(&buf->lock);
    return 0;
}

int traffic_buffer_dequeue(TrafficBuffer* buf, IntersectionData* out) {
    pthread_mutex_lock(&buf->lock);
    while (buf->size == 0 && simulation_running) {
        // wait for data or shutdown
        pthread_cond_wait(&buf->not_empty, &buf->lock);
    }
    if (buf->size == 0 && !simulation_running) {
        // shutting down
        pthread_mutex_unlock(&buf->lock);
        return -1;
    }
    *out = buf->buffer[buf->front];
    buf->front = (buf->front + 1) % buf->capacity;
    buf->size--;
    pthread_cond_signal(&buf->not_full);
    pthread_mutex_unlock(&buf->lock);
    return 0;
}

void traffic_buffer_destroy(TrafficBuffer* buf) {
    free(buf->buffer);
    pthread_mutex_destroy(&buf->lock);
    pthread_cond_destroy(&buf->not_empty);
    pthread_cond_destroy(&buf->not_full);
    buf->buffer = NULL;
    buf->size   = buf->front = buf->rear = 0;
}


// #include "traffic.h"
// #include <stdio.h>


// // Initialize the traffic buffer
// int traffic_buffer_init(TrafficBuffer* buffer, size_t capacity) {
//     buffer->front = 0;
//     buffer->rear = 0;
//     buffer->size = 0;
//     return 0; // Success
// }

// // Enqueue an item into the buffer
// int traffic_buffer_enqueue(TrafficBuffer* buffer, IntersectionData item) {
//     if (buffer->size == BUFFER_SIZE) {
//         return -1;  // Buffer full
//     }
//     buffer->buffer[buffer->rear] = item;  // Store item at the rear
//     buffer->rear = (buffer->rear + 1) % BUFFER_SIZE;
//     buffer->size++;
//     return 0;  // Success
// }

// // Dequeue an item from the buffer
// int traffic_buffer_dequeue(TrafficBuffer* buffer, IntersectionData* out) {
//     if (buffer->size == 0) {
//         return -1;  // Buffer empty
//     }
//     *out = buffer->buffer[buffer->front];  // Get the front item
//     buffer->front = (buffer->front + 1) % BUFFER_SIZE;
//     buffer->size--;
//     return 0;  // Success
// }

// // Destroy the buffer (clear it)
// void traffic_buffer_destroy(TrafficBuffer* buffer) {
//     buffer->front = 0;
//     buffer->rear = 0;
//     buffer->size = 0;
// }

