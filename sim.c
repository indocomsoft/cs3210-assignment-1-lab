#include "sim.h"

// #define DEBUG
// #define PRINT_TRAIN_OUTPUT

int main()
{
    input_t* input = (input_t*)malloc(sizeof(input_t));

    read_input(input);
#ifdef DEBUG
    print_input(input);
#endif

    run_simulation(input);

    cleanup_input(input);

    return 0;
}

void run_simulation(input_t* input)
{
    omp_set_dynamic(0);
    omp_set_num_threads(input->total_trains);
    printf("total trains: %d\n", input->total_trains);

    timekeeper_t** track_timekeepers = setup_track_timekeepers(input);
    timekeeper_t* station_timekeepers = setup_station_timekeepers(input);
    int threads;

#pragma omp parallel
    {
        threads = omp_get_num_threads();
    }
    printf("Running simulation with %d threads\n", threads);

    int cur_time = 0;
#pragma omp parallel shared(track_timekeepers, station_timekeepers)
    {
        train_t* train;
        int tid = omp_get_thread_num();

        if (tid >= input->green_line->start_train_id && tid < input->yellow_line->start_train_id) {
            train = build_train(tid, input->green_line);
        } else if (tid >= input->yellow_line->start_train_id && tid < input->blue_line->start_train_id) {
            train = build_train(tid, input->yellow_line);
        } else {
            train = build_train(tid, input->blue_line);
        }
        while (cur_time < input->ticks) {
            if ((int)train->spawn_time == cur_time) {
                train->spawned = true;
                train->next_state = OPEN_DOOR;
                train->next_door_open_duration = (int) ceil((double)input->popularity[train->station_id] * (rand() % 10 + 1));
                train->next_state_time = timekeeper_increase_by(&station_timekeepers[train->station_id], train->next_door_open_duration, cur_time);
                print_debug_train_status(cur_time, input->station_names, train, -1, "spawn");
            }

            while (train->next_state_time >= 0 && train->next_state_time <= cur_time) {
                if (train->next_state == OPEN_DOOR) {
                    station_stat_open_door(&train->line->stats[train->line_station_id], cur_time, train->next_door_open_duration, train->travelling_forward);
                    train->next_state = CLOSE_DOOR;
                    train->next_state_time += train->next_door_open_duration;
                    print_debug_train_status(cur_time, input->station_names, train, -1, "open door");
                } else {
                    int next_line_station_idx = next_line_station_id(train);
                    int next_station_id = train->line->stations[next_line_station_idx];
                    if (train->next_state == CLOSE_DOOR) {
                        int travel_time = input->stations[train->station_id][next_station_id];
                        train->next_travel_duration = travel_time;
                        train->next_state_time = timekeeper_increase_by(&track_timekeepers[train->station_id][next_station_id], travel_time, cur_time);
                        train->next_state = DEPART;
                        print_debug_train_status(cur_time, input->station_names, train, -1, "close door");
                    } else if (train->next_state == DEPART) {
                        train->next_state = ARRIVE;
                        train->next_state_time += train->next_travel_duration;
                        print_debug_train_status(cur_time, input->station_names, train, next_station_id, "departed");
                    } else if (train->next_state == ARRIVE) {
                        train->line_station_id = next_line_station_idx;
                        train->station_id = next_station_id;
                        train->next_state = OPEN_DOOR;
                        train->next_door_open_duration = (int) ceil((double)input->popularity[train->station_id] * (rand() % 10 + 1));
                        train->next_state_time = timekeeper_increase_by(&station_timekeepers[train->station_id], train->next_door_open_duration, cur_time);
                        print_debug_train_status(cur_time, input->station_names, train, -1, "arrived");
                    }
                }
            }

#ifdef PRINT_TRAIN_OUTPUT
// Ensure that all actions to be taken within this window are complete first;
#pragma omp barrier
#pragma omp single
            {
                printf("%d: ", cur_time); // Only print this once
            }
            print_train_status(cur_time, train);
#pragma omp barrier
// Ensure that all train statuses are printed already
#pragma omp single
            {
                printf("\n"); // Only print this once
            }
#endif

#pragma omp barrier
#pragma omp single
            {
                cur_time++;
            }
        }

        destroy_train(train);
        // #pragma omp critical
        //         {
        //             if (tid >= 10 && tid < 20) {
        //                 printf("tid: %d\n", tid);
        //                 printf("cur_time: %d, ", cur_time);
        //                 printf("train name: %s, ", train->name);
        //                 if (train->travelling_forward) {
        //                     printf("forward: true, ");
        //                 } else {
        //                     printf("forward: false, ");
        //                 }
        //                 printf("train position on line: %d, ", train->line_station_id);
        //                 printf("current station: %s, ", input->station_names[train->station_id]);
        //                 printf("next state time: %lf, ", train->next_state_time);
        //                 printf("%lf\n", train->spawn_time);
        //             }
        //         }
    }
    cleanup_station_timekeepers(station_timekeepers);
    cleanup_track_timekeepers(track_timekeepers, input);
    print_stats_input(input);
}

void print_train_status(int cur_time, train_t* train)
{
#pragma omp critical
    {
        int next_line_station_idx;
        int next_station_id;

        if (train->spawned == true) {
            switch (train->next_state) {
            case OPEN_DOOR:
            case CLOSE_DOOR:
            case DEPART:
                printf("%s-s%d, ", train->name, train->station_id);
                break;
            case ARRIVE:
                next_line_station_idx = next_line_station_id(train);
                next_station_id = train->line->stations[next_line_station_idx];
                printf("%s-s%d->s%d, ", train->name, train->station_id, next_station_id);
            }
        }
    }
}

void print_debug_train_status(int cur_time, char** station_names, train_t* train, int next_station_id, char* status)
{
#ifdef DEBUG
#pragma omp critical
    {
        printf("| %-3d | %-5s | %-15s | %-15s | %-15s | %-4d \n",
            cur_time,
            train->name,
            station_names[train->station_id],
            next_station_id > 0 ? station_names[next_station_id] : "",
            status,
            train->next_state_time);
    }
#endif
}

void cleanup_track_timekeepers(timekeeper_t** timekeepers, input_t* input)
{
    int i;
    for (i = 0; i < input->num_stations; i++) {
        free(timekeepers[i]);
    }
    free(timekeepers);
}

void cleanup_station_timekeepers(timekeeper_t* timekeepers)
{
    free(timekeepers);
}

timekeeper_t** setup_track_timekeepers(input_t* input)
{
    int i, j;

    timekeeper_t** timekeepers = (timekeeper_t**)malloc(sizeof(timekeeper_t*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        timekeepers[i] = (timekeeper_t*)malloc(sizeof(timekeeper_t) * input->num_stations);
        for (j = 0; j < input->num_stations; j++) {
            timekeeper_init(&timekeepers[i][j]);
        }
    }

    return timekeepers;
}

timekeeper_t* setup_station_timekeepers(input_t* input)
{
    int i;
    timekeeper_t* timekeepers = (timekeeper_t*)malloc(sizeof(timekeeper_t) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        timekeeper_init(&timekeepers[i]);
    }
    return timekeepers;
}
