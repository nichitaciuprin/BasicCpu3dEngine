#include <stdint.h>

uint64_t GetPid();
int GetCpuUsage();
long GetTime();
void GetConsolePosition(int* x, int* y);
void Halt(unsigned long milliseconds);
void FixedTimeStart();
void FixedTimeEnd();
