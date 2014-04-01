#ifndef DRIVER_H
#define DRIVER_H
typedef struct {
	int (*start)();
	int (*stop)();
} driver_t;
#endif