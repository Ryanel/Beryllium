#ifndef TIMER_H
#define TIMER_H
#include <log.h>
#include <types.h>
void timer_recieveTick(uint32_t device_id);
uint32_t timer_getHi();
uint32_t timer_getLow();
uint32_t timer_getDevice();
const char* timer_getDeviceString(uint32_t device);
uint32_t timer_getDeviceRes();
#endif
