#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include <pthread.h>
#include "task.h"

typedef struct {
    List* tasks;
    int   next_task_id;
    pthread_mutex_t lock;
} TaskManager;

TaskManager* task_manager_create(void);

void task_manager_destroy(TaskManager* mgr);

bool task_manager_add(TaskManager* mgr, const char* name, int priority);

bool task_manager_remove(TaskManager* mgr, int task_id);

bool task_manager_set_status(TaskManager* mgr, int task_id, TaskStatus status);

size_t task_manager_start_all_pending(TaskManager* mgr);

const char* task_status_to_string(TaskStatus status);

void task_manager_print(const TaskManager* mgr);

#endif