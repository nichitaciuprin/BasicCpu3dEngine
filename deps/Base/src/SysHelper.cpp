#if defined(_WIN64)
    #include "SysHelperWin.cpp"
#elif defined(unix)
    #include "SysHelperLinux.cpp"
#else
    #error "SysHelper not supported"
#endif