#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// 定义整数比较函数
int int_cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

// 定义整数释放函数
void int_free(void *data) {
    free(data);
}

int main() {
    printf("任务管理器链表示例\n");
    
    // 创建一个整数链表
    List *int_list = init_list(int_cmp, int_free);
    
    // 添加一些任务ID
    for (int i = 1; i <= 5; i++) {
        int *task_id = malloc(sizeof(int));
        *task_id = i * 100;
        insert_at_tail(int_list, task_id);
        printf("添加任务ID: %d\n", *task_id);
    }
    
    printf("\n链表大小: %zu\n", get_length(int_list));
    
    // 查找任务ID 300
    int search_id = 300;
    ListNode *found = search_by_value(int_list, &search_id);
    if (found) {
        printf("找到任务ID: %d\n", *(int*)found->data);
    }
    
    // 删除任务ID 200
    int delete_id = 200;
    if (delete_by_value(int_list, &delete_id)) {
        printf("删除任务ID: %d\n", delete_id);
    }
    
    printf("\n剩余任务数: %zu\n", get_length(int_list));
    
    // 打印所有任务
    printf("\n所有任务ID: ");
    ListNode *current = int_list->head;
    while (current) {
        printf("%d ", *(int*)current->data);
        current = current->next;
    }
    printf("\n");
    
    // 清理
    destroy_list(int_list);
    
    return 0;
}