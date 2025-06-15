#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static uint64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL);
}

static uint32_t calculate_hyperperiod(Scheduler* scheduler) {
    uint32_t hyperperiod = scheduler->tasks[0]->period_ms;
    for (int i = 1; i < scheduler->num_tasks; i++) {
        uint32_t a = hyperperiod;
        uint32_t b = scheduler->tasks[i]->period_ms;
        // Calculate LCM
        while (b != 0) {
            uint32_t t = b;
            b = a % b;
            a = t;
        }
        hyperperiod = (hyperperiod * scheduler->tasks[i]->period_ms) / a;
    }
    return hyperperiod;
}

void scheduler_init(Scheduler* scheduler) {
    scheduler->num_tasks = 0;
    scheduler->is_running = false;
    scheduler->hyperperiod_ms = 0;
}

void scheduler_add_task(Scheduler* scheduler, Task* task) {
    if (scheduler->num_tasks < MAX_TASKS) {
        scheduler->tasks[scheduler->num_tasks++] = task;
        scheduler->hyperperiod_ms = calculate_hyperperiod(scheduler);
    }
}

void scheduler_start(Scheduler* scheduler) {
    uint64_t cycle_start = get_current_time_ms();
    scheduler->is_running = true;

    // Initialize next arrival times
    for (int i = 0; i < scheduler->num_tasks; i++) {
        scheduler->tasks[i]->next_arrival = cycle_start;
    }

    while (scheduler->is_running) {
        uint64_t current_time = get_current_time_ms();
        
        // Check each task
        for (int i = 0; i < scheduler->num_tasks; i++) {
            Task* task = scheduler->tasks[i];
            
            if (task->is_active && current_time >= task->next_arrival) {
                // Execute task
                task->execute(task);
                
                // Calculate next arrival
                do {
                    task->next_arrival += task->period_ms;
                } while (task->next_arrival <= current_time);
            }
        }

        // Sleep for a short time to prevent busy waiting
        usleep(1000); // 1ms
    }
}

void scheduler_stop(Scheduler* scheduler) {
    scheduler->is_running = false;
}