#ifndef IO_H_
#define IO_H_

#include <stdbool.h>

void print_menu(void);
void print_intermediate_stats(int processed_count, int len, double avg_len, 
                             int in_num, double avg_stay_time);
void print_final_stats(double total_time, double downtime, int in_count_1, 
                      int processed_1, int type2_requests, double error);

#endif