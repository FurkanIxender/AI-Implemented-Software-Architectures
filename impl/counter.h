#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>
#include <pthread.h>

typedef struct {
    uint32_t value;
    pthread_mutex_t mutex;
} Counter;

void counter_init(Counter* counter);
void counter_increment(Counter* counter);
uint32_t counter_get_value(Counter* counter);

#endif