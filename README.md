# 双向链表实现 (Doubly Linked List Implementation)

一个功能完整、高效的双向链表数据结构实现，使用纯C语言编写，包含丰富的操作函数和全面的测试套件。

## 📋 功能特性

### 基础功能
- ✅ 双向链表节点管理（前驱/后继指针）
- ✅ 动态内存分配和释放
- ✅ 泛型数据支持（任意数据类型）
- ✅ 类型安全回调函数

### 插入操作
- 头部插入 (`insert_at_head`)
- 尾部插入 (`insert_at_tail`)
- 指定位置插入 (`insert_at_position`)
- 节点前插入 (`insert_before_node`)
- 节点后插入 (`insert_after_node`)

### 删除操作
- 删除头节点 (`delete_at_head`)
- 删除尾节点 (`delete_at_tail`)
- 按值删除 (`delete_by_value`)
- 按位置删除 (`delete_at_position`)
- 删除指定节点 (`delete_node`)

### 搜索操作
- 正向搜索 (`search_by_value`)
- 反向搜索 (`search_by_value_reverse`)
- 按位置获取 (`get_node_at_position`)
- 反向按位置获取 (`get_node_at_position_reverse`)

### 更新操作
- 按值更新 (`update_by_value`)
- 节点更新 (`update_node`)
- 条件更新 (`update_if`)

### 实用功能
- 链表清空 (`clear_list`)
- 链表销毁 (`destroy_list`)
- 空链表检查 (`is_empty`)
- 获取长度 (`get_length`)

## 🏗️ 项目结构

```
Task_Manager/
├── include/
│   └── list.h           # 链表头文件（接口定义）
├── src/
│   └── list.c           # 链表实现源文件
├── test/
│   └── test_list.c      # 全面的测试套件
├── main.c               # 示例使用程序
├── Makefile             # 构建配置文件
├── README.md            # 项目说明文件
└── build/               # 构建输出目录（自动生成）
```

## 🔧 构建与运行

### 环境要求
- GCC 编译器
- Make 构建工具
- Valgrind（用于内存检查，可选）

### 编译项目

```bash
# 编译主程序
make

# 编译测试程序
make test

# 清理构建文件
make clean

# 重新构建
make rebuild
```

### 运行程序

```bash
# 运行主程序示例
./task_manager

# 运行完整的测试套件
./test_list
```

## 🧪 测试与验证

### 包含的测试
- ✅ 基本功能测试
- ✅ 节点插入测试
- ✅ 搜索功能测试
- ✅ 删除功能测试
- ✅ 更新功能测试
- ✅ 字符串链表测试
- ✅ 边界条件测试
- ✅ 性能测试
- ✅ 综合测试

### 内存泄漏检查

```bash
# 检查主程序内存泄漏
make memcheck

# 检查测试程序内存泄漏
make test-memcheck

# 快速内存检查
make quick-check
```

### 示例输出
```
开始全面测试双向链表实现...
使用Valgrind测试内存泄漏...

=== 测试1：基本功能 ===
✓ 整数链表初始化成功
✓ 尾部插入第一个元素成功
✓ 头部插入第二个元素成功
✓ 指定位置插入成功
✓ 链表销毁成功

=== 测试2：节点插入 ===
✓ 基础链表创建成功
✓ 在节点后插入成功
✓ 在节点前插入成功
✓ 链表顺序正确

...（更多测试输出）...

================================
所有测试通过！链表实现正确。
注意：请使用'make memcheck'检查内存泄漏
================================
```

## 📖 使用示例

### 基本用法

```c
#include <stdio.h>
#include <stdlib.h>
#include "include/list.h"

// 整数比较函数
int int_cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

// 整数释放函数
void int_free(void *data) {
    free(data);
}

int main() {
    // 初始化链表
    List *list = init_list(int_cmp, int_free);
    
    // 插入数据
    for (int i = 0; i < 5; i++) {
        int *num = malloc(sizeof(int));
        *num = i * 10;
        insert_at_tail(list, num);
    }
    
    // 搜索数据
    int key = 30;
    ListNode *found = search_by_value(list, &key);
    if (found) {
        printf("Found: %d\n", *(int*)found->data);
    }
    
    // 更新数据
    int new_value = 99;
    update_node(list, list->head, &new_value, 
                [](void *target, const void *new_val) {
                    *(int*)target = *(int*)new_val;
                });
    
    // 条件删除（删除偶数）
    int delete_count = 0;
    ListNode *current = list->head;
    while (current) {
        ListNode *next = current->next;
        if (*(int*)current->data % 2 == 0) {
            delete_node(list, current);
            delete_count++;
        }
        current = next;
    }
    
    // 销毁链表
    destroy_list(list);
    
    return 0;
}
```

### 字符串链表示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/list.h"

// 字符串比较函数
int str_cmp(const void *a, const void *b) {
    return strcmp((char*)a, (char*)b);
}

// 字符串释放函数
void str_free(void *data) {
    free(data);
}

int main() {
    // 初始化字符串链表
    List *list = init_list(str_cmp, str_free);
    
    // 插入字符串
    insert_at_tail(list, strdup("Apple"));
    insert_at_tail(list, strdup("Banana"));
    insert_at_tail(list, strdup("Cherry"));
    
    // 搜索字符串
    char *search_key = "Banana";
    ListNode *found = search_by_value(list, search_key);
    if (found) {
        printf("Found: %s\n", (char*)found->data);
    }
    
    // 遍历链表
    ListNode *current = list->head;
    while (current) {
        printf("%s -> ", (char*)current->data);
        current = current->next;
    }
    printf("NULL\n");
    
    // 销毁链表
    destroy_list(list);
    
    return 0;
}
```

## 🔍 内存管理

本项目实现了完善的内存管理机制：

1. **自动内存释放**：链表销毁时自动释放所有节点数据
2. **自定义释放函数**：支持用户定义的数据释放逻辑
3. **内存泄漏检测**：提供Valgrind配置文件
4. **边界检查**：所有操作都包含空指针和边界检查

### 内存检查结果示例
```
==213577== HEAP SUMMARY:
==213577==     in use at exit: 0 bytes in 0 blocks
==213577==   total heap usage: 20,148 allocs, 20,148 frees, 283,245 bytes allocated
==213577== 
==213577== All heap blocks were freed -- no leaks are possible
==213577== 
==213577== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## ⚡ 性能特点

- **时间复杂度**：
  - 插入/删除：O(1)（头尾操作）或 O(n)（指定位置）
  - 搜索：O(n)
  - 更新：O(1)（已知节点）或 O(n)（按值）

- **空间复杂度**：O(n)

- **内存效率**：每个节点额外开销为两个指针（前驱和后继）

## 🐛 调试支持

### 编译选项
```bash
# 调试版本（包含调试信息）
make debug

# 发布版本（优化）
make release
```

### 调试功能
- 详细的错误消息和堆栈跟踪
- 边界条件检查
- 断言验证

## 🤝 贡献指南

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 开发规范
- 遵循 C99 标准
- 使用 4 空格缩进
- 函数和变量使用蛇形命名法
- 为所有公共函数添加文档注释

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 📞 联系方式

如有问题或建议，请通过以下方式联系：
- 提交 GitHub Issue
- 发送电子邮件至 [903458367@qq.com]

## 🎯 未来计划

- [ ] 添加迭代器支持
- [ ] 实现链表排序功能
- [ ] 添加序列化/反序列化支持
- [ ] 支持多线程安全版本
- [ ] 添加更多的算法示例

---

**注意**：本项目适合学习和教育用途，也可作为C语言数据结构的参考实现。生产环境使用前请进行充分的测试和评估。