#include <mutex.h>
#include <types.h>
void mutex_lock(mutex_t *mutex)
{
	if (mutex)
	{
		while(mutex)
		{
			asm("pause");
		}
	}
	mutex = 1;
}

void mutex_unlock(mutex_t *mutex)
{
	if (mutex)
	{
		mutex = 0;
	}
	else
	{
		//mutex->value = false;
	}
}

void mutex_init(mutex_t *mutex)
{
	mutex = 0;
}