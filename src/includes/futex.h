#ifndef FUTEX_H
#define FUTEX_H
#include <stdio.h>
typedef struct {
	signed long status;
} futex_t;

inline int futex_up(futex_t *futex)
{
	futex->status = futex->status + 1;
	printf("futex_t up: %d\n",futex->status);
}
inline int futex_down(futex_t *futex)
{
	futex->status = futex->status - 1;
	printf("futex_t down: %d\n",futex->status);
}

#endif