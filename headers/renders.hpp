#ifndef RENDERS_HPP
#define RENDERS_HPP

#include <iostream>
#include "raylib.h"
#include "constants.hpp"
#include "house.hpp"

namespace RENDER{
    void RenderHouse(){
        DrawCube({0,0,1}, 1.2f, 0.5, 2, RED);
    }
}

#endif