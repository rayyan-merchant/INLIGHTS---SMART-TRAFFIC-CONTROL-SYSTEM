#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"

#define MAX_ROWS 1000
// In dataset.c or main.c
// int current_green = 0;   // 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
// int total_vehicles = 0;

// Function to read the dataset from a file
int read_dataset(const char *filename, VehicleData dataset[MAX_ROWS]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    char line[256];
    int row = 0;

    // Skip the header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Parse the line
        sscanf(line, "%*s %d %d %d %d %d %d %d %d", 
               &dataset[row].north_vehicle, 
               &dataset[row].south_vehicle, 
               &dataset[row].east_vehicle, 
               &dataset[row].west_vehicle, 
               &dataset[row].north_emg, 
               &dataset[row].south_emg, 
               &dataset[row].east_emg, 
               &dataset[row].west_emg);

        row++;
        if (row >= MAX_ROWS) break;
    }

    fclose(file);
    return row; // Return the number of rows read
}

