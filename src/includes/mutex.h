#ifndef MUTEX_H
#define MUTEX_H
#include <types.h>
typedef struct {
	bool value;
} mutex_t;
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);
void mutex_init(mutex_t *mutex);
#endif