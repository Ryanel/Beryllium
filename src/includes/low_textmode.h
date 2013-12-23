//Low Level Display Drivers
#ifndef LOW_TEXTMODE_H
#define LOW_TEXTMODE_H
#include <types.h>
void textmode_write(int x,int y,uint8_t data);
void textmode_write_color(int x,int y,uint8_t data, uint8_t attr);
uint8_t textmode_read(int x,int y);
void textmode_scroll(int from,int to);
void textmode_setcursor(int x,int y);
#endif
