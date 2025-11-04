#ifndef PRINT_H__
#define PRINT_H__

void print_menu(void);
void print_head(void);
void print_structure_info(void);
void print_country(const country_t *c);
void print_data(const country_t countries[], const size_t number_countries);
void print_key_table(const key_t key_table[], const size_t number_countries);
void print_by_key(const key_t key_table[], const country_t countries[], const size_t number_countries);

#endif