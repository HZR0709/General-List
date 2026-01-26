#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "task_manager.h"

#define PRODUCER_THREADS 4
#define WORKER_THREADS   2
#define RUN_SECONDS      5

static TaskManager* g_mgr;
static volatile int g_running = 1;

/* ---------- 生产者线程：并发 add ---------- */
void* producer_thread(void* arg) {
    long tid = (long)arg;
    int count = 0;

    while (g_running) {
        char name[64];
        snprintf(name, sizeof(name), "task-%ld-%d", tid, count++);

        task_manager_add(g_mgr, name, rand() % 10);
        usleep(1000);  // 减缓一点，避免刷爆
    }

    return NULL;
}

/* ---------- 工作线程：并发 update ---------- */
void* worker_thread(void* arg) {
    (void)arg;

    while (g_running) {
        task_manager_start_all_pending(g_mgr);
        usleep(2000);
    }

    return NULL;
}

/* ---------- 监控线程：并发读 ---------- */
void* monitor_thread(void* arg) {
    (void)arg;

    while (g_running) {
        printf("\n====== SNAPSHOT ======\n");
        task_manager_print(g_mgr);
        usleep(500000);  // 0.5s 打印一次
    }

    return NULL;
}

int main(void) {
    srand((unsigned)time(NULL));

    g_mgr = task_manager_create();
    if (!g_mgr) {
        fprintf(stderr, "Failed to create TaskManager\n");
        return 1;
    }

    pthread_t producers[PRODUCER_THREADS];
    pthread_t workers[WORKER_THREADS];
    pthread_t monitor;

    /* 启动生产者 */
    for (long i = 0; i < PRODUCER_THREADS; ++i) {
        pthread_create(&producers[i], NULL, producer_thread, (void*)i);
    }

    /* 启动工作线程 */
    for (int i = 0; i < WORKER_THREADS; ++i) {
        pthread_create(&workers[i], NULL, worker_thread, NULL);
    }

    /* 启动监控线程 */
    pthread_create(&monitor, NULL, monitor_thread, NULL);

    /* 运行一段时间 */
    sleep(RUN_SECONDS);
    g_running = 0;

    /* 等待所有线程退出 */
    for (int i = 0; i < PRODUCER_THREADS; ++i) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < WORKER_THREADS; ++i) {
        pthread_join(workers[i], NULL);
    }

    pthread_join(monitor, NULL);

    task_manager_destroy(g_mgr);
    return 0;
}
