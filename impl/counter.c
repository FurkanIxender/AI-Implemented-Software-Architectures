#include "counter.h"


void counter_init(Counter* counter) {
    counter->value = 0;
    pthread_mutex_init(&counter->mutex, NULL);
}

void counter_increment(Counter* counter) {
    pthread_mutex_lock(&counter->mutex);
    counter->value++;
    pthread_mutex_unlock(&counter->mutex);
}

uint32_t counter_get_value(Counter* counter) {
    uint32_t val;
    pthread_mutex_lock(&counter->mutex);
    val = counter->value;
    pthread_mutex_unlock(&counter->mutex);
    return val;
}