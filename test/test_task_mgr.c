#include "task_manager.h"
#include <stdio.h>
#include <assert.h>

void test_task_mgr(void) {
    TaskManager* mgr = task_manager_create();

    assert(task_manager_add(mgr, "test_1", 3));
    assert(task_manager_add(mgr, "test_2", 1));
    assert(task_manager_add(mgr, "test_3", 4));
    assert(task_manager_add(mgr, "test_4", 2));
    assert(task_manager_add(mgr, "test_5", 4));
    printf("✓ 成功添加5个任务\n");

    printf("初始任务列表：\n");
    task_manager_print(mgr);

    assert(task_manager_start_all_pending(mgr) != 0);
    printf("✓ 成功启动所有 pending\n");

    assert(task_manager_remove(mgr, 3));
    printf("✓ 成功删除一个任务\n");

    printf("更新后的任务列表:\n");
    task_manager_print(mgr);
}

int main() {
    printf("开始测试任务管理器实现...\n");

    test_task_mgr();

    printf("\n================================\n");
    printf("所有测试通过！任务管理器实现正确。\n");
    printf("================================\n");

    return 0;
}