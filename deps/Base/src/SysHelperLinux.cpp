#include <time.h>
#include <errno.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

bool setInitSpec = true;
struct timespec initSpec;

long clock2()
{
    return clock()/(CLOCKS_PER_SEC/1000);
}

long GetTime()
{
    if (setInitSpec)
    {
        setInitSpec = false;
        clock_gettime(CLOCK_REALTIME, &initSpec);
        return 0;
    }

    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    unsigned long ms1 = initSpec.tv_sec * 1000 + round(initSpec.tv_nsec / 1.0e6);
    unsigned long ms2 =     spec.tv_sec * 1000 + round(    spec.tv_nsec / 1.0e6);

    long ms = ms2 - ms1;

    return ms;
}

int GetCpuUsage()
{
    // TODO implement
    return 100;
}


void Halt(unsigned long milliseconds)
{
    // TODO review

    struct timespec ts;
    int res;

    if (milliseconds < 0)
        errno = EINVAL;

    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    }
    while (res && errno == EINTR);

    // usleep(milliseconds * 1000);
}
