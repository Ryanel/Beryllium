#ifndef VIDEO_H
#define VIDEO_H

#include <types.h>
void video_init();
///Inits graphics
int video_graphics_init();
///Prints a char
void video_printchar(int x,int y, unsigned char c);
///Prints a char w/ attribute byte
void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute);
///Scrolls
void video_scroll(int from,int to);
///Set's cursor to x and y
void video_setcursor(int x,int y);
///Set current attribute to back & fire
void video_setattributetext(unsigned char back, unsigned char fore);
///Resets current color attribute
void video_resetattributetext();
///Can we use graphics?
int video_graphics_capable();
#endif
