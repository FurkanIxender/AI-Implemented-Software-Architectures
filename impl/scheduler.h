#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

typedef struct {
    Task* tasks[2];
    pthread_t scheduler_thread;
    bool is_running;
} Scheduler;

void* scheduler_function(void* arg);  // Add function declaration
void scheduler_init(Scheduler* scheduler, Task* task1, Task* task2);
void scheduler_start(Scheduler* scheduler);
void scheduler_stop(Scheduler* scheduler);

#endif