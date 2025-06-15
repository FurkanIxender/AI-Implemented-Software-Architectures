#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL);
}

void task_init(Task* task, Counter* counter, TaskFunction execute,
               uint32_t period_ms, uint32_t deadline_ms, const char* name) {
    task->counter = counter;
    task->execute = execute;
    task->period_ms = period_ms;
    task->deadline_ms = deadline_ms;
    task->next_arrival = 0;
    task->is_active = true;
    task->name = name;
}

void task_update_period(Task* task, uint32_t new_period_ms) {
    task->period_ms = new_period_ms;
}

void task_start(Task* task) {
    task->is_running = true;
    task->next_arrival = get_current_time_ms();
    pthread_create(&task->thread, NULL, task_function, task);
}