#ifndef FUTEX_H
#define FUTEX_H

typedef struct {
	signed long status;
} futex_t;

inline int futex_up(futex_t *futex)
{
	futex->status = futex->status + 1;
}
inline int futex_down(futex_t *futex)
{
	futex->status = futex->status - 1;
}

#endif