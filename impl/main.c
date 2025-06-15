#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scheduler.h"
#include "task.h"
#include "counter.h"

void* task_function(void* arg) {
    Task* task = (Task*)arg;
    
    while (task->is_running) {
        pthread_mutex_lock(&task->mutex);
        
        // Wait for scheduler signal and check if it's time for next arrival
        uint64_t current_time = get_current_time_ms();
        while (current_time < task->next_arrival) {
            struct timespec ts;
            ts.tv_sec = task->next_arrival / 1000;
            ts.tv_nsec = (task->next_arrival % 1000) * 1000000;
            pthread_cond_timedwait(&task->cond, &task->mutex, &ts);
            current_time = get_current_time_ms();
        }
        
        // Execute task
        counter_increment(task->counter);
        printf("%s: Counter value: %u (Time: %lu ms)\n", 
               task->name, counter_get_value(task->counter), 
               current_time);
        
        // Calculate next arrival time
        task->next_arrival += task->period_ms;
        pthread_mutex_unlock(&task->mutex);
    }
    
    return NULL;
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

int main() {
    Counter counter;
    counter_init(&counter);
    
    Task task1, task2;
    task_init(&task1, &counter, 1000, "Thread1"); // 1 second period
    task_init(&task2, &counter, 2000, "Thread2"); // 2 second period
    
    Scheduler scheduler;
    scheduler_init(&scheduler, &task1, &task2);
    
    scheduler_start(&scheduler);
    task_start(&task1);
    task_start(&task2);
    
    // Let it run for 10 seconds
    sleep(10);
    
    // Change periods
    printf("Changing periods...\n");
    task_set_period(&task1, 500);  // 0.5 second
    task_set_period(&task2, 1500); // 1.5 seconds
    
    // Run for another 10 seconds
    sleep(10);
    
    scheduler_stop(&scheduler);
    task_stop(&task1);
    task_stop(&task2);
    
    return 0;
}