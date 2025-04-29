#ifndef DATASET_H
#define DATASET_H
// In dataset.h or a similar header file
extern int current_green;      // To keep track of the current green direction
extern int total_vehicles;     // To keep track of total vehicles

typedef struct {
    int north_vehicle;
    int south_vehicle;
    int east_vehicle;
    int west_vehicle;
    int north_emg;
    int south_emg;
    int east_emg;
    int west_emg;
} VehicleData;

#endif // DATASET_H

