#ifndef SORT_COUNTRY_H__
#define SORT_COUNTRY_H__

void bubble_sort(country_t countries[], const size_t number_countries);
void quick_sort(country_t countries[], int low, int high);
void test_sorting(const country_t countries[], const size_t number_countries);

#endif