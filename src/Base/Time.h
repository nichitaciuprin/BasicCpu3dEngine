#pragma once

typedef LONGLONG TICKS;

TICKS TicksPerSecondCache = 0;
TICKS TicksPerMillisecondCache = 0;
TICKS FixedTimeStepCache = 0;
TICKS TimeOld = 0;
TICKS TimeNew = 0;
TICKS PerfTimeStart = 0;
TICKS PerfTimeEnd = 0;

TICKS TicksPerSecond()
{
    if (TicksPerSecondCache == 0)
    {
        LARGE_INTEGER perfFreq;
        QueryPerformanceFrequency(&perfFreq);
        auto perfCounterFrequency = perfFreq.QuadPart;
        TicksPerSecondCache = perfCounterFrequency;
    }
    return TicksPerSecondCache;
}
TICKS TicksPerMillisecond()
{
    if (TicksPerMillisecondCache == 0)
        TicksPerMillisecondCache = TicksPerSecond()/1000;
    return TicksPerMillisecondCache;
}
TICKS FixedTimeStep()
{
    if (FixedTimeStepCache == 0)
        FixedTimeStepCache = TicksPerMillisecond()*20;
    return FixedTimeStepCache;
}
TICKS GetTime()
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    return ticks.QuadPart;
}
TICKS GetCalcTime(TICKS oldTime, TICKS newTime)
{
    if (oldTime > newTime)
       return LLONG_MAX - oldTime + newTime;
    return newTime - oldTime;
}
size_t ToMilliseconds(TICKS ticks)
{
    return (ticks/TicksPerMillisecond());
}
void CheckFPS()
{
    if (TimeOld == 0)
    {
        TimeOld = GetTime();
        return;
    }

    TimeNew = GetTime();

    auto diff = GetCalcTime(TimeOld,TimeNew);

    TimeOld = TimeNew;

    auto milliseconds = ToMilliseconds(diff);
    if (milliseconds == 0) return;

    auto fps = 1000/milliseconds;
    if (fps < 240)
        printf("=== FPS:%zu ===\n",fps);
}
void CheckPerfStart()
{
    PerfTimeStart = GetTime();
}
void CheckPerfEnd()
{
    PerfTimeEnd = GetTime();

    auto diff = GetCalcTime(PerfTimeStart,PerfTimeEnd);
    auto milliseconds = ToMilliseconds(diff);
    cout << milliseconds << "ms" << endl;
    // cout << "ABORT CALLED" << endl;
    // abort();
}
void WaitAfterRender()
{
    TICKS waitTicks = 0;
    if (TimeOld == 0)
    {
        waitTicks = FixedTimeStep()/TicksPerMillisecond();
    }
    else
    {
        TimeNew = GetTime();
        auto calcTime = GetCalcTime(TimeOld, TimeNew);
        auto duno = FixedTimeStep();
        waitTicks = duno - calcTime;
        if (waitTicks < 0)
            waitTicks = 0;
    }
    auto milliseconds = (DWORD)(waitTicks/TicksPerMillisecond());
    Sleep(milliseconds);
    TimeOld = GetTime();
}