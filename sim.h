#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "line.h"
#include "station_stat.h"
#include "timekeeper.h"
#include "train.h"

timekeeper_t** setup_track_timekeepers(input_t*);
timekeeper_t* setup_station_timekeepers(input_t*);
void cleanup_track_timekeepers(timekeeper_t** timekeepers, input_t* input);
void cleanup_station_timekeepers(timekeeper_t* timekeepers);
void run_simulation(input_t*);
void print_debug_train_status(int cur_time, char** station_names, train_t* train, int next_station_id, char* status);
void print_train_status(int cur_time, train_t* train);
