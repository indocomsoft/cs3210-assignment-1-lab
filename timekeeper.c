#include "timekeeper.h"

void timekeeper_init(timekeeper_t* timekeeper)
{
    pthread_mutex_init(&timekeeper->lock, NULL);
    timekeeper->time = 0;
}

int timekeeper_increase_by(timekeeper_t* timekeeper, int duration, int current_time)
{
    pthread_mutex_lock(&timekeeper->lock);
    int start = max(current_time, timekeeper->time);
    timekeeper->time = start + duration;
    pthread_mutex_unlock(&timekeeper->lock);
    return start;
}
