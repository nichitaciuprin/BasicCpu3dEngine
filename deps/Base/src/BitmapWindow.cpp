#if defined(_WIN64)
    #include "BitmapWindowWin.cpp"
#elif defined(unix)
    #include "BitmapWindowLinux.cpp"
#else
    #error "BitmapWindow not supported"
#endif
