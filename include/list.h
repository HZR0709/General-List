#ifndef __LIST_H
#define __LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*update_fn)(void *data, const void *new_value);
typedef bool (*predicate_fn)(const void *data);

typedef struct ListNode {
    void *data;             // 数据域
    struct ListNode *prev;  // 前驱指针
    struct ListNode *next;  // 后继指针
} ListNode;

typedef struct {
    ListNode *head;     // 头指针
    ListNode *tail;     // 尾指针
    size_t size;        // 链表长度 

    // 函数指针
    int (*cmp)(const void *a, const void *b);// 比较（查找 / 删除）
    void (*free_data)(void *data);       // 销毁数据
} List;
 
// 创建新节点
ListNode* create_node(void* data);

// 初始化双链表
List* init_list(int (*cmp)(const void *, const void *), void (*free_data)(void *));

// 是否为空
bool is_empty(List* list);

// 获取链表长度
size_t get_length();

// 插入函数
ListNode* insert_at_tail(List* list, void* data);        // 尾插
ListNode* insert_at_head(List* list, void* data);        // 头插
ListNode* insert_at_position(List* list, void* data, int position);    // 在指定位置插入
ListNode* insert_after_node(List* list, ListNode* target, void* data); // 在指定节后插入
ListNode* insert_before_node(List* list, ListNode* target, void* data);    // 在指定节点前插入

// 删除
bool delete_at_head(List* list);    // 头删
bool delete_at_tail(List* list);    // 尾删
bool delete_by_value(List* list, void* key);   // 删除指定值节点
bool delete_at_position(List* list, int position);    // 删除指定位置的节点
bool delete_node(List* list, ListNode* node);                     // 删除指定节点

// 查找
ListNode* search_by_value(List* list, void* key);           // 按值查找节点
ListNode* search_by_value_reverse(List* list, void* key);   // 反向按值查找
ListNode* get_node_at_position(List* list, int position);                 // 获取指定位置节点
ListNode* get_node_at_position_reverse(List* list, int position);         // 从后向前获取节点

// 修改
bool update_by_value(List* list, const void *key, const void *new_value, update_fn updater);
bool update_node(List* list, ListNode* node, const void* new_value, update_fn updater);
size_t update_if(List* list, predicate_fn pred, const void* new_value, update_fn updater);

// 其他操作
bool reverse_list(List* list);  // 反转
bool sort_list(List* list);     // 排序
bool merge_sorted_lists(List* list1, List* list2);  // 合并两个有序列表
bool detect_cycle();                                // 检测环
bool remove_duplicates();                           // 去重
bool find_middle();                                 // 找到中间节点
bool get_nth_from_end();                            // 获取倒数第N个节点
bool swap_nodes(ListNode* node1, ListNode* node2);  // 交换两个节点

// 内存管理
void clear_list(List* list);    // 清空链表
void destroy_list(List* list);  // 销毁链表（释放所有内存）

// 工具函数
bool copy_list(List* dest_list, List* src_list);    // 复制链表
bool compare_lists();                               // 比较两个链表
bool concat_lists(List* list1, List* list2);        // 连接两个链表 



#endif    