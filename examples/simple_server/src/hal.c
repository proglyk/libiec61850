#include "hal.h"
#include <time.h>
#include "stm32f4xx_hal.h"

#if defined __linux__ || defined __APPLE__ /*__linux__WIN32__STM32__*/

#if defined __MACH__ || defined __linux__ /*__MACH____linux__*/

#include <sys/time.h>

#if defined __MACH__
#define CLOCK_REALTIME 1
#endif


int
clock_gettime(int dummy, struct timespec* t)
{
	struct timeval now;
	int rv = gettimeofday(&now, NULL);
	if (rv) return rv;
	t->tv_sec = now.tv_sec;
	t->tv_nsec = now.tv_usec * 1000;
}

#endif /*__MACH____linux__*/

uint64_t
Hal_getTimeInMs()
{
	struct timespec tp;

	clock_gettime(CLOCK_REALTIME, &tp);

	return tp.tv_sec * 1000 + (tp.tv_nsec / 1000000);
}

#elif defined __WIN32__ /*__linux__WIN32__STM32__*/
#include "windows.h"

uint64_t
Hal_getTimeInMs()
{
	FILETIME ft;
	uint64_t ll_now;
	GetSystemTimeAsFileTime(&ft);

	ll_now = (LONGLONG)ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime) << 32LL);

	return ll_now;
}
#elif defined __STM32__ /*__linux__WIN32__STM32__*/

uint64_t
Hal_getTimeInMs()
{
	uint64_t ll_now;

	ll_now = (uint64_t)HAL_GetTick();

	return ll_now;
}

#endif /*__linux__WIN32__STM32__*/
