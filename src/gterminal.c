#include <types.h>
#include <beryllium/video.h>
#include <log.h>
#define FONT_WIDTH  8
#define FONT_HEIGHT 8

int term_x = 0;
int term_y = 0;

void gterminal_draw_char(int x, int y, char c)
{
    //int x_offset = x * FONT_WIDTH;
    //int y_offset = y * FONT_HEIGHT;
    //klog(LOG_DEBUG,"GTM","Attempting to write '%c' to the screen at (%d, %d)!\n",c,x_offset,y_offset);
    for(int yi = 0; yi<FONT_HEIGHT; yi++)
    {
        for(int xi = 0; xi<FONT_WIDTH; xi++)
        {
            //video_draw_pixel(xi + x_offset, yi + y_offset, 0x10,0x10,0x10);
        }
    }
}


void gterminal_draw_string(int x, int y, char* input) 
{

}
 
void gterminal_printc(unsigned char c)
{
    video_draw_pixel(100, 100, 0xFF,0xFF,0xFF);
    //gterminal_draw_char(term_x,term_y,c);
}

void gterminal_print(const char * c)
{

}