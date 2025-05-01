#include "traffic.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize the traffic buffer
int traffic_buffer_init(TrafficBuffer* buffer, size_t capacity) {
    buffer->buffer = (IntersectionData*)malloc(capacity * sizeof(IntersectionData));
    if (!buffer->buffer) {
        return -1;  // Allocation failed
    }
    
    buffer->capacity = capacity;  // Store the capacity
    buffer->front = 0;
    buffer->rear = 0;
    buffer->size = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
    return 0;  // Success
}

// Enqueue an item into the buffer
int traffic_buffer_enqueue(TrafficBuffer* buffer, IntersectionData item) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->size == buffer->capacity) {  // Buffer full, wait
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);
    }
    buffer->buffer[buffer->rear] = item;  // Store item at the rear
    buffer->rear = (buffer->rear + 1) % buffer->capacity;
    buffer->size++;
    pthread_cond_signal(&buffer->not_empty);  // Signal that buffer is not empty
    pthread_mutex_unlock(&buffer->mutex);
    return 0;  // Success
}

// Dequeue an item from the buffer
int traffic_buffer_dequeue(TrafficBuffer* buffer, IntersectionData* out) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->size == 0) {  // Buffer empty, wait
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
    }
    *out = buffer->buffer[buffer->front];  // Get the front item
    buffer->front = (buffer->front + 1) % buffer->capacity;
    buffer->size--;
    pthread_cond_signal(&buffer->not_full);  // Signal that buffer is not full
    pthread_mutex_unlock(&buffer->mutex);
    return 0;  // Success
}

// Destroy the buffer (clear it)
void traffic_buffer_destroy(TrafficBuffer* buffer) {
    pthread_mutex_lock(&buffer->mutex);
    free(buffer->buffer);
    buffer->buffer = NULL;
    buffer->front = 0;
    buffer->rear = 0;
    buffer->size = 0;
    buffer->capacity = 0;
    pthread_mutex_unlock(&buffer->mutex);
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->not_full);
    pthread_cond_destroy(&buffer->not_empty);
}