#include "data_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_traffic_data(const char* filename, Traffic* traffic_array) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open traffic data file");
        return -1;
    }

    char line[256];
    int line_num = 0;

    // Skip the header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && line_num < 4) { // Only first 4 entries (North, South, East, West)
        Traffic t = {0};
        char* token = strtok(line, ",");

        int column = 0;
        while (token != NULL) {
            if (column >= 1 && column <= 4) {
                t.cars += atoi(token);
            } else if (column >= 5 && column <= 8) {
                t.cars += atoi(token);
            } else if (column >= 9 && column <= 12) {
                t.cars += atoi(token);
            } else if (column >= 13 && column <= 16) {
                t.cars += atoi(token);
            }
            token = strtok(NULL, ",");
            column++;
        }

        traffic_array[line_num] = t;
        line_num++;
    }

    fclose(file);
    return 0;
}

