#ifndef STATION_STAT_DEFINED
#define STATION_STAT_DEFINED

#define max(a, b) (a >= b ? a : b)
#define min(a, b) (a <= b ? a : b)

typedef struct station_stat_t station_stat_t;

#include "timekeeper.h"
#include <pthread.h>
#include <stdbool.h>

#define STATION_STAT_FORWARD 0
#define STATION_STAT_REVERSE 1

struct station_stat_t {
    pthread_mutex_t lock;
    int num_door_opening[2];
    int total_wait_time[2];
    int min_wait_time[2];
    int max_wait_time[2];
    int last_closed_time[2];
};

void station_stat_init(station_stat_t* station_stat);
void station_stat_open_door(station_stat_t*, int current_time, int duration, bool forward);
#endif
