#ifndef PROC_COUNTRY_H__
#define PROC_COUNTRY_H__

int load_data(country_t countries[], size_t *number_countries);
int save_data(const country_t countries[], const size_t number_countries);
int add_record(country_t countries[], size_t *number_countries);
int delete_country(country_t countries[], size_t *number_countries);
int search_by_sport(const country_t countries[], const size_t number_countries);

#endif