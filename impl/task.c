#include "task.h"
#include <stdlib.h>
#include <time.h>

static uint64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL);
}

void task_init(Task* task, Counter* counter, uint32_t period_ms, const char* name) {
    task->counter = counter;
    task->period_ms = period_ms;
    task->is_running = false;
    task->name = name;
    task->next_arrival = 0;
    pthread_mutex_init(&task->mutex, NULL);
    pthread_cond_init(&task->cond, NULL);
}

void task_start(Task* task) {
    task->is_running = true;
    task->next_arrival = get_current_time_ms();
    pthread_create(&task->thread, NULL, task_function, task);
}

void task_stop(Task* task) {
    task->is_running = false;
    pthread_join(task->thread, NULL);
}

void task_set_period(Task* task, uint32_t new_period_ms) {
    pthread_mutex_lock(&task->mutex);
    task->period_ms = new_period_ms;
    pthread_mutex_unlock(&task->mutex);
}