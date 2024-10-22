#if defined(_WIN64)
    #include "NetHelperWin.cpp"
#elif defined(unix)
    #include "NetHelperLinux.cpp"
#else
    #error "NetHelper not supported"
#endif
