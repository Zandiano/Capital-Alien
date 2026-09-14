#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

#define array_size(arr) int(sizeof((arr))/sizeof((arr)[0]))

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

#define _WHITE               {255, 255, 255, 255}
#define _BLACK               {0, 0, 0, 255}
#define _RED                 {255, 0, 0, 255}
#define _GREEN               {0, 255, 0, 255}
#define _BLUE                {0, 0, 255, 255}
#define _PINK                {255, 77, 186, 255}
#define _YELLOW              {255, 250, 77, 255}
#define _PURPLE              {151, 77, 255, 255}
#define _PURPLEISH_PINK      {223, 78, 200, 255}
#define _DREAMISTIC_INDIGO   {89, 0, 255, 255}
#define _LAVENDER            {199, 159, 239, 255}
#define _AMARANTH            {229, 43, 80, 255}

typedef enum{
    white, black, red, green, blue, pink, yellow, purple, purpleish, indigo, lavender, amaranth
} CA_Colors;

Color all_colors[] = {_WHITE, _BLACK, _RED, _GREEN, _BLUE, _PINK, _YELLOW, _PURPLE, _PURPLEISH_PINK, _DREAMISTIC_INDIGO, _LAVENDER, _AMARANTH};

Color* GetColor(CA_Colors color){
    return &all_colors[color];
}

#endif