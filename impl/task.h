#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "counter.h"

typedef struct Task Task;
typedef void (*TaskFunction)(Task* task);

struct Task {
    TaskFunction execute;
    Counter* counter;
    uint32_t period_ms;
    uint32_t deadline_ms;
    uint64_t next_arrival;
    bool is_active;
    const char* name;
};

void task_init(Task* task, Counter* counter, TaskFunction execute, 
               uint32_t period_ms, uint32_t deadline_ms, const char* name);
void task_update_period(Task* task, uint32_t new_period_ms);

#endif