#include "data_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int read_traffic_data(const char* filename, TrafficBuffer* buffer) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open dataset file");
        return 0;
    }

    char line[256];
    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read all rows and enqueue them into the buffer
    while (fgets(line, sizeof(line), file)) {
        IntersectionData data;
        if (parse_line(line, &data) == 0) {
            traffic_buffer_enqueue(buffer, data);
        }
    }

    // Add a sentinel value to indicate end of data
    IntersectionData sentinel = { .minute_of_day = -1 };
    traffic_buffer_enqueue(buffer, sentinel);

    fclose(file);
    return 1;
}
// int read_traffic_data(const char* filename, Traffic* traffic_array) {
//     FILE* file = fopen(filename, "r");
//     if (!file) {
//         perror("Failed to open dataset file");
//         return 0;
//     }

//     char line[256];
//     //int line_num = 0;

//     // Skip the header line
//     fgets(line, sizeof(line), file);

//     // Only read the first row (for simulation initialization)
//     if (fgets(line, sizeof(line), file)) {
//         int normal[4] = {0};  // NORTH, SOUTH, EAST, WEST
//         int emergency[4] = {0};

//         // Tokenize the CSV line
//         char* token = strtok(line, ",");
//         int column = 0;
//         while (token != NULL && column < 9) {
//             int value = atoi(token);
//             switch (column) {
//                 case 1: normal[NORTH] = value; break;
//                 case 2: normal[SOUTH] = value; break;
//                 case 3: normal[EAST]  = value; break;
//                 case 4: normal[WEST]  = value; break;
//                 case 5: emergency[NORTH] = value; break;
//                 case 6: emergency[SOUTH] = value; break;
//                 case 7: emergency[EAST]  = value; break;
//                 case 8: emergency[WEST]  = value; break;
//                 default: break;
//             }
//             token = strtok(NULL, ",");
//             column++;
//         }

//         // Store values into traffic_array
//         traffic_array[NORTH].normal_vehicles = normal[NORTH];
//         traffic_array[NORTH].emergency_vehicles = emergency[NORTH];

//         traffic_array[SOUTH].normal_vehicles = normal[SOUTH];
//         traffic_array[SOUTH].emergency_vehicles = emergency[SOUTH];

//         traffic_array[EAST].normal_vehicles = normal[EAST];
//         traffic_array[EAST].emergency_vehicles = emergency[EAST];

//         traffic_array[WEST].normal_vehicles = normal[WEST];
//         traffic_array[WEST].emergency_vehicles = emergency[WEST];
//     }

//     fclose(file);
//     return 1;
// }

