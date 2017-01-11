#include <windows.h>
#include <time.h>

#include "util.h"

#define EPOCHFILETIME (116444736000000000LL)

#include "timer.h"

int gettimeofday(struct timeval * tv, struct timezone *)
{
    FILETIME        ft;
    LARGE_INTEGER   li;
    __int64           t;

    if (tv)
    {
        GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;       /* In 100-nanosecond intervals */
        t -= EPOCHFILETIME;     /* Offset to the Epoch time */
        t /= 10;                /* In microseconds */
        tv->tv_sec  = (long)(t / 1000000);
        tv->tv_usec = (long)(t % 1000000);
    }

    return 0;

}

#include "timer_inc.cpp"

