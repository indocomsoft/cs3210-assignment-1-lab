#include "input.h"

void read_input(input_t* input)
{
    int i, j;

    scanf("%d\n", &(input->num_stations));

    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);

    // STATION NAMES
    input->station_names = (char**)malloc(sizeof(char*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->station_names[i] = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH);
    }

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    const char* delim = "\n,";
    char* next;
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        strcpy(input->station_names[i], next);
        i++;
    }

    // STATIONS
    input->stations = (int**)malloc(sizeof(int*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->stations[i] = (int*)malloc(sizeof(int) * input->num_stations);
    }

    for (i = 0; i < input->num_stations; i++) {
        fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
        j = 0;
        for (next = strtok(str_buf, " "); next != NULL; next = strtok(NULL, " ")) {
            input->stations[i][j] = atoi(next);
            j++;
        }
    }

    // POPULARITY
    input->popularity = (double*)malloc(sizeof(int*) * input->num_stations);
    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%lf", &(input->popularity[i]));
        i++;
    }

    // GREEN LINE
    input->green_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->green_line, input);
    input->green_line->id = 'g';

    // YELLOW LINE
    input->yellow_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->yellow_line, input);
    input->yellow_line->id = 'y';

    // BLUE LINE
    input->blue_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->blue_line, input);
    input->blue_line->id = 'b';

    // TICKS
    scanf("%d\n", &(input->ticks));

    // NUM TRAINS
    input->num_trains = (int*)malloc(sizeof(int) * 3);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%d", &(input->num_trains[i]));
        i++;
    }
    input->green_line->num_trains = input->num_trains[0];
    input->green_line->start_train_id = 0;
    input->yellow_line->num_trains = input->num_trains[1];
    input->yellow_line->start_train_id = input->num_trains[0];
    input->blue_line->num_trains = input->num_trains[2];
    input->blue_line->start_train_id = input->num_trains[0] + input->num_trains[1];

    input->total_trains = input->blue_line->start_train_id + input->blue_line->num_trains;

    // CLEANUP
    free(str_buf);
}

void print_input(input_t* input)
{
    int i;
    int j;
    printf("%d\n", input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        printf("%s ", input->station_names[i]);
    }
    printf("\n");

    for (i = 0; i < input->num_stations; i++) {
        for (j = 0; j < input->num_stations; j++) {
            printf("%d ", input->stations[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < input->num_stations; i++) {
        printf("%.2lf ", input->popularity[i]);
    }
    printf("\n");

    print_line(input->green_line);
    print_line(input->yellow_line);
    print_line(input->blue_line);

    printf("%d\n", input->ticks);

    for (i = 0; i < 3; i++) {
        printf("%d ", input->num_trains[i]);
    }
    printf("\n");
}

void cleanup_input(input_t* input)
{
    int i;
    for (i = 0; i < input->num_stations; i++) {
        free(input->station_names[i]);
    }
    free(input->station_names);
    for (i = 0; i < input->num_stations; i++) {
        free(input->stations[i]);
    }
    free(input->stations);
    free(input->popularity);
    free(input->green_line->stations);
    free(input->green_line);
    free(input->yellow_line->stations);
    free(input->yellow_line);
    free(input->blue_line->stations);
    free(input->blue_line);
    free(input->num_trains);
    free(input);
}

void print_stats_input(input_t* input)
{
    // Green
    printf("green: ");
    print_stats_line(input->green_line);
    // Yellow
    printf("yellow: ");
    print_stats_line(input->yellow_line);
    // Blue
    printf("blue: ");
    print_stats_line(input->blue_line);
}
