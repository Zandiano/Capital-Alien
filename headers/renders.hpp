#ifndef RENDERS_HPP
#define RENDERS_HPP

#include <iostream>
#include "raylib.h"
#include "constants.hpp"
#include "house.hpp"
#include "tabletop.hpp"

namespace RENDER{
    void RenderHouse(TBL::Game game){
        DrawCube({0,0,1}, 1.2f, 0.5, 2, GetHouseColor(*GetHouse(game, GetPos(*GetPlayer(game)))));
    }

    void RenderName(TBL::Game game){
        DrawText(GetName(*GetPlayer(game)).c_str(), 0, 0, 32, GetPlayerColor(*GetPlayer(game)));
    }

    void RenderRound(TBL::Game game){
        DrawText(to_string(GetRound(game)).c_str(), CONSTANT::ScreenW-80, 0, 40, RED);
    }
}

#endif