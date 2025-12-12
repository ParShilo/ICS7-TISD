#ifndef COMPARE_H__
#define COMPARE_H__

void delete_str_duplicates(char *str);
int search_in_str(const char *str, char key);
void create_ordered_str(char *str, int size);
void create_random_str(char *str, int size);
node_t *create_tree_from_string(const char *str);
void compare_delete(void);
void compare_find(void);
void compare_memory(void);

#endif