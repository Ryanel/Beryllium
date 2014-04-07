#include <beryllium/vterm.h>
#include <stdlib.h>

term_t* term_create(int width, int height, int isPhysical)
{
	term_t* build     = malloc(sizeof(term_t));

	build->height     = height;
	build->width      = width;
	build->isPhysical = isPhysical;
	build->isInit     = 1;
	build->data       = malloc((height * width));
	return build;
}
void term_write(term_t *term,int x,int y,char c)
{
	term->data[(term->width * y) + x] = c;
}
void term_writes(term_t *term,int x,int y,char *c)
{
	int i = 0;
	while (c[i])
	{
		term->data[(term->width * y++) + x++] = c[i++];
	}
}
char term_read(term_t *term,int x,int y)
{
	return term->data[(term->width * y) + x];
}
