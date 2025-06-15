#ifndef TASK_H
#define TASK_H

#include <pthread.h>
#include <stdbool.h>
#include "counter.h"

typedef struct {
    pthread_t thread;
    Counter* counter;
    uint32_t period_ms;
    bool is_running;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    const char* name;
    uint64_t next_arrival;  // Add next arrival time
} Task;

void* task_function(void* arg);  // Add the function declaration
void task_init(Task* task, Counter* counter, uint32_t period_ms, const char* name);
void task_start(Task* task);
void task_stop(Task* task);
void task_set_period(Task* task, uint32_t new_period_ms);

#endif