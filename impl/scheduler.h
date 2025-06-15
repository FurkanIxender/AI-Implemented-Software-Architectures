#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <stdbool.h>

#define MAX_TASKS 2

typedef struct {
    Task* tasks[MAX_TASKS];
    int num_tasks;
    bool is_running;
    uint32_t hyperperiod_ms;
} Scheduler;

void scheduler_init(Scheduler* scheduler);
void scheduler_add_task(Scheduler* scheduler, Task* task);
void scheduler_start(Scheduler* scheduler);
void scheduler_stop(Scheduler* scheduler);

#endif