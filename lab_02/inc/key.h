#ifndef KEY_H__
#define KEY_H__

void initialize_key_table(key_t key_table[], const country_t countries[], const size_t number_countries);
void bubble_sort_key(key_t key_table[], const size_t number_countries);
void quick_sort_key(key_t key_table[], int low, int high);

#endif