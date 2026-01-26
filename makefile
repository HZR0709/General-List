# 编译器设置
CC := gcc
CFLAGS := -Wall -g -Wextra -Iinclude -pthread
TARGET := task_manager
TEST_LIST_TARGET := test_list
TEST_TASK_MGR_TARGET := test_task_mgr
TEST_MULTITHREAD_TARGET := test_multithread

# 主程序源文件
SRCS := src/list.c \
		src/task_manager.c \
		src/task.c \
        main.c

# 测试程序源文件
TEST_LIST_SRCS := src/list.c \
                  test/test_list.c

TEST_TASK_MGR_SRCS := src/list.c \
                      src/task.c \
                      src/task_manager.c \
                      test/test_task_mgr.c

TEST_MULTITHREAD_SRCS := src/list.c \
                         src/task.c \
                         src/task_manager.c \
                         test/test_multithread.c

# 构建目录
BUILD_DIR := build
TEST_BUILD_DIR := $(BUILD_DIR)/test

# 目标文件路径
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
TEST_LIST_OBJS := $(addprefix $(TEST_BUILD_DIR)/, $(TEST_LIST_SRCS:.c=.o))
TEST_TASK_MGR_OBJS := $(addprefix $(TEST_BUILD_DIR)/, $(TEST_TASK_MGR_SRCS:.c=.o))
TEST_MULTITHREAD_OBJS := $(addprefix $(TEST_BUILD_DIR)/, $(TEST_MULTITHREAD_SRCS:.c=.o))

# 依赖文件
DEPS := $(OBJS:.o=.d)
TEST_LIST_DEPS := $(TEST_LIST_OBJS:.o=.d)
TEST_TASK_MGR_DEPS := $(TEST_TASK_MGR_OBJS:.o=.d)
TEST_MULTITHREAD_DEPS := $(TEST_MULTITHREAD_OBJS:.o=.d)

# 默认目标
all: $(BUILD_DIR) $(TARGET)

# 测试目标 - 构建所有测试
test: test-list test-task-mgr test-multithread
 
# 单独构建链表测试
test-list: $(TEST_BUILD_DIR) $(TEST_LIST_TARGET)

# 单独构建任务管理器测试
test-task-mgr: $(TEST_BUILD_DIR) $(TEST_TASK_MGR_TARGET)

# 单独构建多线程测试
test-multithread: $(TEST_BUILD_DIR) $(TEST_MULTITHREAD_TARGET)

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)/src
	mkdir -p $(TEST_BUILD_DIR)/test

# 可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(TEST_LIST_TARGET): $(TEST_LIST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_LIST_OBJS)

$(TEST_TASK_MGR_TARGET): $(TEST_TASK_MGR_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_TASK_MGR_OBJS)

$(TEST_MULTITHREAD_TARGET): $(TEST_MULTITHREAD_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_MULTITHREAD_OBJS)

# 编译主程序目标文件
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译测试程序目标文件
$(TEST_BUILD_DIR)/%.o: %.c | $(TEST_BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 生成依赖文件
$(BUILD_DIR)/%.d: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MM $< -MT "$(BUILD_DIR)/$*.o $@" > $@

$(TEST_BUILD_DIR)/%.d: %.c | $(TEST_BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MM $< -MT "$(TEST_BUILD_DIR)/$*.o $@" > $@

# 清理
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_LIST_TARGET) $(TEST_TASK_MGR_TARGET) $(TEST_MULTITHREAD_TARGET)

# 清理并重新构建
rebuild: clean all

# ==================== 内存检查选项 ====================

# 主程序内存检查
memcheck: $(TARGET)
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TARGET)

# 链表测试内存检查
test-list-memcheck: test-list
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TEST_LIST_TARGET)

# 任务管理器测试内存检查
test-task-mgr-memcheck: test-task-mgr
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TEST_TASK_MGR_TARGET)

# 多线程测试内存检查（注意：valgrind支持多线程，但可能需要特殊处理）
test-multithread-memcheck: test-multithread
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         --suppressions=/usr/share/valgrind/default.supp \
	         ./$(TEST_MULTITHREAD_TARGET)

# 使用helgrind检测线程竞争条件
test-multithread-helgrind: test-multithread
	valgrind --tool=helgrind \
	         ./$(TEST_MULTITHREAD_TARGET)

# 使用drd检测线程错误
test-multithread-drd: test-multithread
	valgrind --tool=drd \
	         ./$(TEST_MULTITHREAD_TARGET)

# 所有测试内存检查
test-all-memcheck: test
	@echo "================ 检查链表测试内存 ================"
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TEST_LIST_TARGET) 2>&1 | grep -A 20 "ERROR SUMMARY"
	@echo "\n================ 检查任务管理器测试内存 ================"
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TEST_TASK_MGR_TARGET) 2>&1 | grep -A 20 "ERROR SUMMARY"
	@echo "\n================ 检查多线程测试内存 ================"
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         --suppressions=/usr/share/valgrind/default.supp \
	         ./$(TEST_MULTITHREAD_TARGET) 2>&1 | grep -A 20 "ERROR SUMMARY"

# 快速内存检查
quick-check: $(TARGET)
	valgrind --leak-check=summary ./$(TARGET)

# ==================== 调试选项 ====================

# 调试版本
debug: CFLAGS += -O0 -DDEBUG
debug: clean all

# 发布版本
release: CFLAGS += -O3 -DNDEBUG
release: clean all

# ==================== 多线程专用选项 ====================

# 构建所有多线程相关的测试和分析
thread-tests: test-multithread test-multithread-helgrind test-multithread-drd
	@echo "多线程测试构建完成"

# 运行多线程测试（带线程安全检测）
run-thread-test: test-multithread
	./$(TEST_MULTITHREAD_TARGET)

# 包含依赖文件
-include $(DEPS)
-include $(TEST_LIST_DEPS)
-include $(TEST_TASK_MGR_DEPS)
-include $(TEST_MULTITHREAD_DEPS)

.PHONY: all test test-list test-task-mgr test-multithread clean \
		rebuild debug release memcheck test-memcheck \
		test-all-memcheck quick-check thread-tests run-thread-test \
		test-multithread-memcheck test-multithread-helgrind test-multithread-drd