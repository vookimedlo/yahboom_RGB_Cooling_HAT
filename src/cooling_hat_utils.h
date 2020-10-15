#ifndef COOLING_UTILS_H
#define COOLING_UTILS_H

#if defined(NDEBUG)
    #define DEBUG_PRINT(...)
#else
    #include <stdio.h>
    #define DEBUG_PRINT(...) \
        do { } while(fprintf(stderr, __VA_ARGS__))
#endif // DEBUG

#endif //COOLING_UTILS_H
