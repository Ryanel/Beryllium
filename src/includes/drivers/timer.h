#ifndef TIMER_H
#define TIMER_H
#include <log.h>
#include <types.h>
void recieveTick(uint32_t device_id);
uint32_t getTimerHi();
uint32_t getTimerLow();
#endif
