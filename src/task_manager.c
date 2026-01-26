#include "task_manager.h"
#include <time.h>
#include <string.h>

TaskManager* task_manager_create(void) {
    TaskManager* mgr = malloc(sizeof(TaskManager));
    if (!mgr) return NULL;

    mgr->tasks = init_list(task_cmp, task_free);
    mgr->next_task_id = 0;

    return mgr;
}

void task_manager_destroy(TaskManager* mgr) {
    if (!mgr) return;

    destroy_list(mgr->tasks);
    free(mgr);
}

bool task_manager_add(TaskManager* mgr, const char* name, int priority) {
    if (!mgr || !name) return false;

    Task* task = malloc(sizeof(Task));
    if (!task) return false;

    task->id = mgr->next_task_id++;
    strncpy(task->name, name, sizeof(task->name) - 1);
    task->name[sizeof(task->name) - 1] = '\0';
    task->priority = priority;
    task->status = TASK_PENDING;
    task->created_at = time(NULL);

    if (!insert_at_tail(mgr->tasks, task)) {
        free(task);
        return false;
    }
    
    return true;
}

bool task_manager_remove(TaskManager* mgr, int task_id) {
    if (!mgr) return false;

    return delete_by_value(mgr->tasks, &task_id);
}

bool task_manager_set_status(TaskManager* mgr, int task_id, TaskStatus status) {
    if (!mgr) return false;

    return update_by_value(
        mgr->tasks, 
        &task_id, 
        &status,
        task_update_status
    );
}

size_t task_manager_start_all_pending(TaskManager* mgr) {
    if (!mgr) return 0;

    TaskStatus new_status = TASK_RUNNING;
    size_t count = update_if(
        mgr->tasks,
        task_is_pending,
        &new_status,
        task_update_status
    );

    return count;
}

// 将枚举状态转换为字符串的函数
const char* task_status_to_string(TaskStatus status) {
    switch (status) {
        case TASK_PENDING:   return "PENDING";
        case TASK_RUNNING:   return "RUNNING";
        case TASK_DONE:      return "DONE";
        case TASK_CANCELED:  return "CANCELED";
        default:             return "UNKNOWN";
    }
}

void task_manager_print(const TaskManager* mgr) {
    if (!mgr || !mgr->tasks) return;

    ListNode* cur = mgr->tasks->head;
    int count = 0;

    while (cur && cur->data) {
        Task* task = cur->data;

        printf("\n========== Task #%d ==========\n", ++count);
        printf("ID:          %d\n", task->id);
        printf("Name:        %s\n", task->name);
        printf("Priority:    %d\n", task->priority);
        printf("Status:      %s\n", task_status_to_string(task->status));

        char time_buf[32];
        struct tm* timeinfo = localtime(&task->created_at);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Created at:  %s\n", time_buf);
        cur = cur->next;
    }

    if (count == 0) {
        printf("No tasks in the manager.\n");
    } else {
        printf("\nTotal tasks: %d\n", count);
    }
}