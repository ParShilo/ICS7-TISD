#include "structs.h"
#include "err.h"

uint64_t tick(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

double get_time(int t1, int t2)
{
    double r = (double)rand() / (double)RAND_MAX;
    return (t2 - t1) * r + t1;
}