#ifndef VIDEO_H
#define VIDEO_H

#include <types.h>

int video_graphics_init();

void video_printchar(int x,int y, unsigned char c);

void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute);

void video_scroll(int from,int to);

void video_setcursor(int x,int y);

void video_setattributetext(unsigned char back, unsigned char fore);

void video_resetattributetext();

int video_graphics_capable();
#endif