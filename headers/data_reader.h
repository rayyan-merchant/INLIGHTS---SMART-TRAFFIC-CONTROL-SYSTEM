#ifndef DATA_READER_H
#define DATA_READER_H

#include "traffic.h"

// Reads traffic data from a CSV file into the current_traffic array
int read_traffic_data(const char* filename, Traffic* traffic_array);

#endif // DATA_READER_H

