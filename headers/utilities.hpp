#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

#define array_size(arr) int(sizeof((arr))/sizeof((arr)[0]))

#define MAXHOUSES 255
#define MAXPLAYERS 4

typedef enum{
    M0 = 0,
    M1 = 1,
    M2 = 2,
    M3 = 4,
    M4 = 8,
    M5 = 16,
    M6 = 32,
    M7 = 64,
    M8 = 128,
} BITMASK;

typedef enum{
    NORMAL,
    START,
    PARK,
    QUESTION_MARK,
    PRISION,
    TELEPORT
} HSETYPE;
#endif