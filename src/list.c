#include "list.h"

ListNode* create_node(void* data) { 
    ListNode* node = malloc(sizeof(ListNode));
    if (!node) return NULL;

    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

List* init_list(int (*cmp)(const void *, const void *), void (*free_data)(void *)) {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->cmp = cmp;
    list->free_data = free_data;

    return list;
}

bool is_empty(List* list) {
    return list == NULL ? true : (list->size == 0);
}

size_t get_length(List* list) {
    if (!list) return 0;
    return list->size;
}

ListNode* insert_at_tail(List* list, void* data) {
    if (!list) return NULL;

    ListNode* new_node = create_node(data);
    if (!new_node) {
        return NULL;
    }

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->prev = list->tail;    // 新节点的前驱指针，要指向原来的尾节点
        list->tail->next = new_node;    // 原尾节点的后继指针，原本指向 NULL ，要将它指向新节点
        list->tail = new_node;          // 尾指针，指向新节点
        
    }

    list->size++;
    return new_node;
}

ListNode* insert_at_head(List* list, void* data) {
    if (!list) return NULL;

    ListNode* new_node = create_node(data);
    if (!new_node) {
        return NULL;
    }

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }

    list->size++;
    return new_node;
}

ListNode* insert_at_position(List* list, void* data, int position) {
    if (!list) return NULL;

    if (position < 0 || position > list->size) {
        fprintf(stderr, "Error: Invalid position %d, size is %d\n", 
                position, list->size);
        return NULL;
    }

    // 特殊情况：插入到头部
    if (position == 0) {
        return insert_at_head(list, data);
    }

    // 特殊情况：插入到尾部
    if (position == list->size) {
        return insert_at_tail(list, data);
    }

    ListNode* new_node = create_node(data);
    if (!new_node) {
        return NULL;
    }
    ListNode* current = list->head;
    for (int i = 0; i < position - 1 && current; i++) {
        current = current->next;
    }
    if (!current) {
        list->free_data(new_node->data);
        free(new_node);
        return NULL;
    }

    // 在current之后插入新节点
    new_node->prev = current;
    new_node->next = current->next;

    if (current->next) {
        current->next->prev = new_node;
    }
    current->next = new_node;
    list->size++;

    return new_node;
}

ListNode* insert_after_node(List* list, ListNode* target, void* data) {
    if (!list || !target) return NULL;

    ListNode* new_node = create_node(data);
    if (!new_node) return NULL;

    new_node->prev = target;
    new_node->next = target->next;

    if (target->next) {
        target->next->prev = new_node;
    } else {
        list->tail = new_node;
    }
    target->next = new_node;

    list->size++;
    return new_node; 
}

ListNode* insert_before_node(List* list, ListNode* target, void* data) {
    if (!list || !target) return NULL;

    ListNode* new_node = create_node(data);
    if (!new_node) return NULL;

    new_node->next = target;
    new_node->prev = target->prev;

    if (target->prev) {
        target->prev->next = new_node;
    } else {
        list->head = new_node;
    }

    target->prev = new_node;
    list->size++;

    return new_node;
}

ListNode* search_by_value(List* list, void* key) {
    if (!list || !list->cmp) return NULL;

    for (ListNode *current = list->head; current; current = current->next) {
        if (list->cmp(current->data, key) == 0) {
            return current;
        } 
    }
    return NULL;
}

ListNode* search_by_value_reverse(List* list, void* key) {
    if (!list) return NULL;

    ListNode* current = list->tail;
    while (current) {
        if (list->cmp(current->data, key) == 0) return current;
        current = current->prev;
    }
    return NULL;
}

ListNode* get_node_at_position(List* list, int position) {
    if (!list) return NULL;

    if (position < 0 || position > list->size) {
        fprintf(stderr, "Error: Invalid position %d, size is %d\n", 
                position, list->size);
        return NULL;
    }

    ListNode* current = list->head;
    for (int i = 0; i < position && current; i++) {
        current = current->next;
    }

    if (!current) return NULL;

    return current;
}

ListNode* get_node_at_position_reverse(List* list, int position) {
    if (!list) return NULL;

    if (position < 0 || position > list->size) {
        fprintf(stderr, "Error: Invalid position %d, size is %d\n", 
                position, list->size);
        return NULL;
    }

    ListNode* current = list->tail;
    for (int i = 0; i < position && current; i++) {
        current = current->prev;
    }
    if (!current) return NULL;

    return current;
}

bool delete_at_head(List* list) {
    if (!list || !list->free_data || !list->head) return false;

    if (is_empty(list)) return false;

    ListNode* node = list->head;
    if (list->head == list->tail) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    list->free_data(node->data);
    free(node);
    list->size--;

    return true;
}

bool delete_at_tail(List* list) {
    if (!list || !list->free_data || !list->tail) return false;

    if (is_empty(list)) return false;

    ListNode* node = list->tail;
    if (list->head == list->tail) {
        list->head = list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    list->free_data(node->data);
    free(node);
    list->size--;

    return true;
}

bool delete_by_value(List* list, void* key) {
    if (!list || !list->cmp || !list->free_data) return false;

    ListNode* node = search_by_value(list, key);
    if (!node) return false;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    list->free_data(node->data);
    free(node);
    list->size--;

    return true;
}

bool delete_at_position(List* list, int position) {
    if (!list || !list->free_data) return false;

    if (position < 0 || position >= list->size) {
        fprintf(stderr, "Error: Invalid position %d, size is %d\n", 
                position, list->size);
        return false;
    }

    if (position == 0) {
        return delete_at_head(list);
    }

    if (position == list->size -1) {
        return delete_at_tail(list);
    }

    ListNode* current = list->head;
    for (int i = 0; i < position && current; i++) {
        current = current->next;
    }
    if (!current) return false;

    if (current->prev) {
        current->prev->next = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    }
    
    list->free_data(current->data);
    free(current);
    list->size--;

    return true;
}

bool delete_node(List* list, ListNode* node) {
    if (!list || !node) return false;

    if (list->head == node) {
        return delete_at_head(list);
    }

    if (list->tail == node) {
        return delete_at_tail(list);
    }

    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }

    list->free_data(node->data);
    free(node);
    list->size--;

    return true;
}

bool update_by_value(List* list, const void *key, const void *new_value, update_fn updater) {
    if (!list || !updater || !list->cmp) return false;

    ListNode* current = search_by_value(list, key);
    if (!current) return false;

    updater(current->data, new_value);

    return true;
} 

bool update_node(List* list, ListNode* node, const void* new_value, update_fn updater) {
    if (!list || !node || !updater) return false;

    updater(node->data, new_value);
    return true;
}
   
size_t update_if(List* list, predicate_fn pred, const void* new_value, update_fn updater) {
    if (!list || !pred ||!updater) return 0;

    size_t count = 0;
    for (ListNode* current = list->head; current; current = current->next) {
        if (pred(current->data)) {
            updater(current->data, new_value);
            count++;
        }
    }
    return count;
}

void clear_list(List* list) {
    if (!list || !list->free_data) return;

    ListNode* current = list->head;
    while (current) {
        ListNode* next = current->next;
        list->free_data(current->data);
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
 
void destroy_list(List* list) {
    if (!list) return;
    clear_list(list);
    free(list);
}

