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
    const double NORMAL_WEIGHT = 1.0;
    const double EMERGENCY_WEIGHT = 5.0;

    return (t.normal_vehicles * NORMAL_WEIGHT) +
           (t.emergency_vehicles * EMERGENCY_WEIGHT);
}

int parse_line(const char* line, IntersectionData* out) {
    char* token;
    size_t len = strlen(line);
    char* line_copy = (char*)malloc(len + 1);  // Allocate memory for the copy
    if (!line_copy) {
        return -1;  // Memory allocation failed
    }
    strcpy(line_copy, line);  // Copy the string

    // Expected format: timestamp,north_vehicle,south_vehicle,east_vehicle,west_vehicle,
    //                  north_emg,south_emg,east_emg,west_emg

    // Get timestamp
    token = strtok(line_copy, ",");
    if (!token) {
        free(line_copy);
        return -1;  // Missing timestamp
    }
    out->minute_of_day = time_to_min(token);
    if (out->minute_of_day == -1) {
        free(line_copy);
        return -1;  // Invalid timestamp
    }

    // Parse normal vehicles for each direction (NORTH, SOUTH, EAST, WEST)
    for (int d = 0; d < 4; d++) {
        token = strtok(NULL, ",");
        if (!token) {
            free(line_copy);
            return -1;  // Missing vehicle count
        }
        out->dir[d].normal_vehicles = atoi(token);
    }

    // Parse emergency vehicles for each direction (NORTH, SOUTH, EAST, WEST)
    for (int d = 0; d < 4; d++) {
        token = strtok(NULL, ",");
        if (!token) {
            free(line_copy);
            return -1;  // Missing emergency vehicle count
        }
        out->dir[d].emergency_vehicles = atoi(token);
    }

    free(line_copy);
    return 0;  // Success
}

// // Convert time string "HH:MM" to minutes since midnight
// int time_to_min(const char* hhmm) {
//     int hours, minutes;
//     if (sscanf(hhmm, "%d:%d", &hours, &minutes) != 2) {
//         return -1;  // Invalid time format
//     }
//     if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
//         return -1;  // Invalid time values
//     }
//     return hours * 60 + minutes;
// }

// // Calculate weighted load based on vehicle counts
// double calc_load(Traffic t) {
//     const double CAR_WEIGHT = 1.0;
//     const double BIKE_WEIGHT = 0.5;
//     const double TRUCK_WEIGHT = 2.0;
//     const double EMERGENCY_WEIGHT = 5.0;

//     return (t.cars * CAR_WEIGHT) +
//            (t.bikes * BIKE_WEIGHT) +
//            (t.trucks * TRUCK_WEIGHT) +
//            (t.emergency * EMERGENCY_WEIGHT);
// }

// // Parse a CSV line into IntersectionData
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

//     free(line_copy);
//     return 0;  // Success
// }