#include "utils.h"
#include "traffic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Convert time string "HH:MM" to minutes since midnight
int time_to_min(const char* hhmm) {
    int hours, minutes;
    if (sscanf(hhmm, "%d:%d", &hours, &minutes) != 2) {
        return -1;  // Invalid time format
    }
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        return -1;  // Invalid time values
    }
    return hours * 60 + minutes;
}

// Calculate weighted load based on vehicle counts
double calc_load(Traffic t) {
    const double CAR_WEIGHT = 1.0;
    const double BIKE_WEIGHT = 0.5;
    const double TRUCK_WEIGHT = 2.0;
    const double EMERGENCY_WEIGHT = 5.0;

    return (t.cars * CAR_WEIGHT) +
           (t.bikes * BIKE_WEIGHT) +
           (t.trucks * TRUCK_WEIGHT) +
           (t.emergency * EMERGENCY_WEIGHT);
}

// Parse a CSV line into IntersectionData

int parse_line(const char* line, IntersectionData* out) {
    char* token;
    char* line_copy = malloc(strlen(line) + 1);
    if (!line_copy) return -1;
    strcpy(line_copy, line);

    // 1) Parse timestamp "HH:MM"
    token = strtok(line_copy, "\t,");  // handle tab or comma if needed
    if (!token) { free(line_copy); return -1; }
    out->minute_of_day = time_to_min(token);
    if (out->minute_of_day < 0) {
        free(line_copy);
        return -1;
    }

    // 2) Read the next 8 numeric fields into an array
    int counts[8];
    for (int i = 0; i < 8; i++) {
        token = strtok(NULL, "\t,");
        if (!token) {
            free(line_copy);
            return -1;
        }
        counts[i] = atoi(token);
    }

    // 3) Assign into the per‐direction Traffic structs
    //    CSV order: north_vehicle, south_vehicle, east_vehicle, west_vehicle,
    //               north_emg,      south_emg,      east_emg,      west_emg
    for (int d = 0; d < 4; d++) {
        out->dir[d].cars      = counts[d];
        out->dir[d].bikes     = 0;
        out->dir[d].trucks    = 0;
        out->dir[d].emergency = counts[4 + d];
    }


// int parse_line(const char* line, IntersectionData* out) {
//     char* token;
//     char* line_copy = strdup(line);  // Make a copy since strtok modifies the string
//     if (!line_copy) {
//         return -1;  // Memory allocation failed
//     }

//     // Expected format: timestamp,north_cars,north_bikes,north_trucks,north_emg,
//     //                  south_cars,south_bikes,south_trucks,south_emg,
//     //                  east_cars,east_bikes,east_trucks,east_emg,
//     //                  west_cars,west_bikes,west_trucks,west_emg

//     // Get timestamp
//     token = strtok(line_copy, ",");
//     if (!token) {
//         free(line_copy);
//         return -1;  // Missing timestamp
//     }
//     out->minute_of_day = time_to_min(token);
//     if (out->minute_of_day == -1) {
//         free(line_copy);
//         return -1;  // Invalid timestamp
//     }

//     // Parse traffic data for each direction
//     for (int d = 0; d < 4; d++) {
//         Traffic* t = &out->dir[d];

//         // Cars
//         token = strtok(NULL, ",");
//         if (!token) {
//             free(line_copy);
//             return -1;
//         }
//         t->cars = atoi(token);

//         // Bikes
//         token = strtok(NULL, ",");
//         if (!token) {
//             free(line_copy);
//             return -1;
//         }
//         t->bikes = atoi(token);

//         // Trucks
//         token = strtok(NULL, ",");
//         if (!token) {
//             free(line_copy);
//             return -1;
//         }
//         t->trucks = atoi(token);

//         // Emergency vehicles
//         token = strtok(NULL, ",");
//         if (!token) {
//             free(line_copy);
//             return -1;
//         }
//         t->emergency = atoi(token);
//     }

    free(line_copy);
    return 0;  // Success
}