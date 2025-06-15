#include "scheduler.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static uint64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL);
}

void* scheduler_function(void* arg) {
    Scheduler* scheduler = (Scheduler*)arg;
    
    while (scheduler->is_running) {
        uint64_t current_time = get_current_time_ms();
        
        // Check and signal tasks that are due
        for (int i = 0; i < 2; i++) {
            Task* current_task = scheduler->tasks[i];
            pthread_mutex_lock(&current_task->mutex);
            
            if (current_time >= current_task->next_arrival) {
                pthread_cond_signal(&current_task->cond);
            }
            
            pthread_mutex_unlock(&current_task->mutex);
        }
        
        // Short sleep to prevent busy waiting
        usleep(1000); // 1ms sleep
    }
    
    return NULL;
}

void scheduler_init(Scheduler* scheduler, Task* task1, Task* task2) {
    scheduler->tasks[0] = task1;
    scheduler->tasks[1] = task2;
    scheduler->is_running = false;
}

void scheduler_start(Scheduler* scheduler) {
    scheduler->is_running = true;
    pthread_create(&scheduler->scheduler_thread, NULL, scheduler_function, scheduler);
}

void scheduler_stop(Scheduler* scheduler) {
    scheduler->is_running = false;
    pthread_join(scheduler->scheduler_thread, NULL);
}