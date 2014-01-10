#ifndef FUTEX_H
#define FUTEX_H

typedef struct {
	signed long status;
} futex_t;

int futex_up(futex_t *futex);
int futex_down(futex_t *futex);

#endif