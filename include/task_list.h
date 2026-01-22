#ifndef __TASK_LIST_H
#define __TASK_LIST_H

#include

typedef enum {
    TASK_TODO,
    TASK_IN_PROGRESS,
    TASK_DONE
} TaskStatus;

typedef struct Task {
    int id;                 // 唯一ID
    char name[64];          // 任务名称
    int priority;           // 优先级
    TaskStatus status;      // 状态
    time_t create_time;     // 创建时间
    time_t due_time;        // 截止时间
    struct Task* next;      // 链表指针
    void (*execute)(struct Task*);  // 函数指针，执行不同类型任务
} Task;

Task* create_task();

void add_task(Task** head, Task* new_task);
int delete_task(Task** head, int id);
Task* find_task(Task* head, int id);
void sort_tasks(Task** head, int (*cmp)(Task*, Task*));
void print_tasks(Task* head);

#endif