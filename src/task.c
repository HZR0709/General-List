#include "task.h"

int task_cmp(const void* a, const void* b) {
    const Task* ta = a;
    const int* id = b;
    return ta->id - *id;
}

void task_free(void* data) {
    Task* t = data;
    free(t);
}

void task_update_status(void* data, const void* new_status) {
    Task* task = data;
    task->status = *(TaskStatus*)new_status;
}

bool task_is_pending(const void* data) {
    if (!data) return false;
    Task* task = (Task*)data;
    if (task->status != TASK_PENDING) return false;
    return true;
}

bool task_is_running(const void* data) {
    if (!data) return false;
    Task* task = (Task*)data;
    if (task->status != TASK_RUNNING) return false;
    return true;
}

bool task_priority_gt_5(const void* data) {
    if (!data) return false;
    Task* task = (Task*)data;
    if (task->priority > 5) return true;
    return false;
}

