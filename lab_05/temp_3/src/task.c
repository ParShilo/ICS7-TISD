#include "task.h"

task_t *create_task(int type)
{
    task_t *task = malloc(sizeof(task_t));
    if (task == NULL)
        return NULL;
    task->type = type;
    task->service_count = 0;
    return task;
}

void delete_task(task_t *task)
{
    free(task);
}