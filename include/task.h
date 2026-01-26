#ifndef __TASK_LIST_H
#define __TASK_LIST_H

#include "list.h"

typedef enum {
    TASK_PENDING,
    TASK_RUNNING,
    TASK_DONE,
    TASK_CANCELED
} TaskStatus;


typedef struct {
    int         id;           // 唯一任务 ID
    char        name[64];     // 任务名
    int         priority;     // 优先级
    TaskStatus status;        // 状态
    time_t      created_at;   // 创建时间
} Task;

int task_cmp(const void* a, const void* b);
void task_free(void* data);
void task_update_status(void* data, const void* new_status);
bool task_is_pending(const void* data);
bool task_is_running(const void* data);
bool task_priority_gt_5(const void* data);

#endif