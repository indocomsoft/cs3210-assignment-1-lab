#include "train.h"

train_t* build_train(int tid, line_t* line)
{
    train_t* train = (train_t*)malloc(sizeof(train_t));
    train->line = line;
    train->id = tid - line->start_train_id;
    if (train->id % 2 == 0) {
        train->travelling_forward = true;
        train->line_station_id = 0;
    } else {
        train->travelling_forward = false;
        train->line_station_id = line->num_stations - 1;
    }
    train->station_id = line->stations[train->line_station_id];
    train->next_state_time = -1;
    train->spawn_time = train->id / 2;
    train->name = (char*)malloc(sizeof(char) * MAX_TRAIN_NAME_LENGTH);
    sprintf(train->name, "%c%d", train->line->id, train->id);

    return train;
}

void destroy_train(train_t* train)
{
    free(train->name);
    free(train);
};

int next_line_station_id(train_t* train)
{
    if (train->line_station_id == 0 && train->travelling_forward == false) {
        train->travelling_forward = true;
        return 1;
    } else if (train->line_station_id == train->line->num_stations - 1 && train->travelling_forward == true) {
        train->travelling_forward = false;
        return train->line->num_stations - 2;
    } else if (train->travelling_forward == true) {
        return train->line_station_id + 1;
    } else {
        return train->line_station_id - 1;
    }
}