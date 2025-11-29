#ifndef _TASK_H_
#define _TASK_H_

#include <stdlib.h>

#define TYPE1 1
#define TYPE2 2

typedef struct
{
    int type;
    int service_count;
} task_t;

task_t *create_task(int type);
void delete_task(task_t *task);

#endif