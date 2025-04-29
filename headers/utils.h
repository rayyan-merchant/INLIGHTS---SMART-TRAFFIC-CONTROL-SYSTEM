#ifndef UTILS_H
#define UTILS_H

#include "traffic.h"

int time_to_min(const char* hhmm);
double calc_load(Traffic t);
int parse_line(const char* line, IntersectionData* out);

#endif

