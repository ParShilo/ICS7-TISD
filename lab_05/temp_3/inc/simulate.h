#ifndef _SIMULATE_H_
#define _SIMULATE_H_

#include "arr.h"
#include "list.h"
#include "task.h"
#include "defines.h"
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>

typedef struct
{
    int tasks_in;
    int tasks_out;
    int failed_tasks;
    int current_length;
    int total_length;
    float total_wait_time;
} SimulationLog;

void print_intermediate_results(SimulationLog *log, int batch_count);
void print_final_results(float total_time, float idle_time, SimulationLog *log1, SimulationLog *log2);
void simulate_arr(int key);
void simulate_list(int key);
float random_float(float min, float max);
float min_float(float a, float b);

#endif