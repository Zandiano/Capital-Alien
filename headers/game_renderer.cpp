#include "game_renderer.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include "constants.hpp"

void RenderHouse(Game game){
    DrawCube({0,0,1}, 1.2f, 0.5, 2, GetHouseColor(GetHouse(game, GetPos(GetPlayer(game)))));
}

void RenderName(Game game){
    DrawText(GetName(GetPlayer(game)).c_str(), 2, 2, 32, GetPlayerColor(GetPlayer(game)));
}

void RenderRound(Game game){
    DrawText(to_string(GetRound(game)).c_str(), ScreenW-80, 0, 40, RED);
}

void RenderMoney(Game game){
    DrawText(to_string(GetMoney(GetPlayer(game))).c_str(), ScreenW-80, 60, 40, RED);
}