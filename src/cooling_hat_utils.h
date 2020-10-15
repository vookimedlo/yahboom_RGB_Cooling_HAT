#ifndef COOLING_UTILS_H
#define COOLING_UTILS_H

#if defined(NDEBUG)
    #define DEBUG_PRINT(...)
#else
    #include <stdio.h>
    #define DEBUG_PRINT(FORMAT, ...) \
        do { fprintf(stderr, FORMAT "\n",  __VA_ARGS__); } while(0)
#endif // DEBUG

#endif //COOLING_UTILS_H
