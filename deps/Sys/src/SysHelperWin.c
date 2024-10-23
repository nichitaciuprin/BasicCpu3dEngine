#include <stdint.h>
#include <process.h>
#include <time.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#include <wincon.h>

static FILETIME idleOld = {};
static FILETIME idleNew = {};
static FILETIME userOld = {};
static FILETIME userNew = {};

static int cpu = 0;
static int cpuTimer = 0;

static long timeStep = 10;
static int endTime = 0;

uint64_t GetPid()
{
    // printf("Process PID:%ld\n", (long)getpid());
    return (uint64_t)getpid();
}
int GetCpuUsage()
{
    // TODO
    if (cpuTimer != 100)
    {
        cpuTimer++;
        return cpu;
    }

    cpuTimer = 0;

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
long GetTime()
{
    // TODO maybe change to QueryPerformanceCounter
    // return clock()/(CLOCKS_PER_SEC/1000);
    return clock();
}
void GetConsolePosition(int* x, int* y)
{
    RECT rect = {};

    HWND handle = GetConsoleWindow();

    // GetClientRect(handle, &rect)
    GetWindowRect(handle, &rect);

    *x = rect.left;
    *y = rect.top;

    // TODO fix this
    *x += 7;
}
void Halt(unsigned long milliseconds)
{
    Sleep(milliseconds);
}
void FixedTimeStart()
{
    endTime = GetTime() + timeStep;
}
void FixedTimeEnd()
{
    long haltTime = endTime - GetTime();
    if (haltTime > 0)
        Halt(haltTime);
    // else
    //     printf("SLOW!\n");
}
