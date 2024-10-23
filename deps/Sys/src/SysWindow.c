#if defined(_WIN64)
    #include "SysWindowWin.cpp"
#elif defined(__unix)
    #include "SysWindowLinux.c"
#else
    #error "SysWindow not supported"
#endif
