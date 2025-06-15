#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scheduler.h"
#include "task.h"
#include "counter.h"

// Task function definitions
void task1_function(Task* task) {
    counter_increment(task->counter);
    printf("%s: Counter value: %u at time %lu ms\n", 
           task->name, 
           counter_get_value(task->counter),
           get_current_time_ms());
}

void task2_function(Task* task) {
    counter_increment(task->counter);
    printf("%s: Counter value: %u at time %lu ms\n", 
           task->name, 
           counter_get_value(task->counter),
           get_current_time_ms());
}

int main() {
    Counter counter;
    counter_init(&counter);
    
    // Create tasks
    Task task1, task2;
    task_init(&task1, &counter, task1_function, 1000, 1000, "Thread1");
    task_init(&task2, &counter, task2_function, 2000, 2000, "Thread2");
    
    // Initialize and setup scheduler
    Scheduler scheduler;
    scheduler_init(&scheduler);
    scheduler_add_task(&scheduler, &task1);
    scheduler_add_task(&scheduler, &task2);
    
    // Start scheduling
    printf("Starting scheduler...\n");
    scheduler_start(&scheduler);
    
    // Let it run for 10 seconds
    sleep(10);
    
    // Change task periods
    printf("Changing periods...\n");
    task_update_period(&task1, 500);
    task_update_period(&task2, 1500);
    
    // Run for another 10 seconds
    sleep(10);
    
    // Stop scheduling
    scheduler_stop(&scheduler);
    
    return 0;
}