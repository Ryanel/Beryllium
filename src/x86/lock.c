#include <mutex.h>
#include <types.h>
void mutex_lock(mutex_t *mutex)
{
	if (mutex->value)
	{
		while(mutex->value)
		{
			asm("pause");
		}
	}
	mutex->value = true;
}

void mutex_unlock(mutex_t *mutex)
{
	if (mutex->value)
	{
		mutex->value = false;
	}
	else
	{
		//mutex->value = false;
	}
}

void mutex_init(mutex_t *mutex)
{
	mutex->value = false;
}