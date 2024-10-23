#if defined(_WIN64)
    #include "BitmapWindowWin.cpp"
#elif defined(__unix)
    #include "BitmapWindowLinux.cpp"
#else
    #error "BitmapWindow not supported"
#endif
