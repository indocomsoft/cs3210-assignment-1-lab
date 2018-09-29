#ifndef INPUT_DEFINED
#define INPUT_DEFINED

typedef struct raw_input_t input_t;

#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATION_NAME_LENGTH 100

struct raw_input_t {
    int num_stations;
    char** station_names;
    int** stations;
    double* popularity;
    line_t* green_line;
    line_t* yellow_line;
    line_t* blue_line;
    int ticks;
    int* num_trains;
    int total_trains;
};

void read_input(input_t*);
void print_input(input_t*);
void cleanup_input(input_t*);
void print_stats_input(input_t*);

#endif
