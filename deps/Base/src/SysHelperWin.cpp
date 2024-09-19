#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <time.h>

FILETIME idleOld = {};
FILETIME idleNew = {};
FILETIME userOld = {};
FILETIME userNew = {};
int cpu = 0;
int count = 0;

int GetCpuUsage()
{
    // TODO
    if (count != 100)
    {
        count++;
        return cpu;
    }

    count = 0;

    FILETIME kernel = {};

    GetSystemTimes(&idleNew, &kernel, &userNew);

    // cpu = GetCpuUsageDiff(idleOld, idleNew, userOld, userNew);

    auto idle = idleNew.dwLowDateTime - idleOld.dwLowDateTime;
    auto user = userNew.dwLowDateTime - userOld.dwLowDateTime;
    auto all = idle + user;
    cpu = all == 0 ? 0 : user * 100 / all;

    idleOld = idleNew;
    userOld = userNew;

    return cpu;
}
void Halt(unsigned long milliseconds)
{
    Sleep(milliseconds);
}
long GetTime()
{
    // return clock()/(CLOCKS_PER_SEC/1000);
    return clock();
}
