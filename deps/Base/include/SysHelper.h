int GetCpuUsage();
void Halt(unsigned long milliseconds);
long GetTime();
void CheckFPS();

// #if defined(_WIN32)
//     #include "SysHelperWin.h"
// #elif defined(__linux__)
//     #include "SysHelperLinux.h"
// #else
//     #error System not supported
// #endif