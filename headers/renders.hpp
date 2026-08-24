#ifndef RENDERS_HPP
#define RENDERS_HPP

#include <iostream>
#include "raylib.h"
#include "constants.hpp"
#include "house.hpp"

namespace RENDER{
    void RenderHouse(HOUSE::HOUSE house){
        DrawCube({0,0,1}, 1.2f, 0.5, 2, house.GetColor());
    }
}

#endif