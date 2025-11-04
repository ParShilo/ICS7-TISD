#ifndef NUM_FUNC_H__
#define NUM_FUNC_H__

int parse_number(char *number_str, number *number, const size_t max_size_mantissa);
void print_number(const number *number);
int multiplication(const number *a, const number *b, number *result);
void delete_lead_zeros(number *number);
void delete_end_zeros(number *number);

#endif // #ifndef NUM_FUNC_H__