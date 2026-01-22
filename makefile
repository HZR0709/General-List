# 编译器设置
CC := gcc
CFLAGS := -Wall -g -Wextra -Iinclude
TARGET := task_manager
TEST_TARGET := test_list

# 主程序源文件
SRCS := src/list.c \
        main.c

# 测试程序源文件
TEST_SRCS := src/list.c \
             test/test_list.c

# 构建目录
BUILD_DIR := build
TEST_BUILD_DIR := $(BUILD_DIR)/test

# 目标文件路径
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
TEST_OBJS := $(addprefix $(TEST_BUILD_DIR)/, $(TEST_SRCS:.c=.o))

# 依赖文件
DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

# 默认目标
all: $(BUILD_DIR) $(TARGET)

# 测试目标
test: $(TEST_BUILD_DIR) $(TEST_TARGET)

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)/src

# 可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS)

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
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

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

# 测试程序内存检查
test-memcheck: test
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TEST_TARGET)

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

# 包含依赖文件
-include $(DEPS)
-include $(TEST_DEPS)

.PHONY: all test clean rebuild debug release \
        memcheck test-memcheck quick-check