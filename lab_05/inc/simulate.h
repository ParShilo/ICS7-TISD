#ifndef SIMULATE_H__
#define SIMULATE_H__

#include "defines.h"
#include "errors.h"
#include "arr.h"
#include "list.h"
#include "print.h"

double generate_random_time(double min, double max);
int simulate_array_queue(int printing, statistics_t *stats_out);
int simulate_list_queue(int printing, statistics_t *stats_out);
int compare_queues(void);
int benchmark_queue_ops(void);

#endif