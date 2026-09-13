#ifndef TABLETOP_HPP
#define TABLETOP_HPP

#include "player.hpp"
#include "house.hpp"

namespace TBL{

    #define MAXHOUSES 120
    #define MAXPLAYERS 4

    typedef struct{
        HSE::House houses[MAXHOUSES] = {0};
        PLR::Player players[MAXPLAYERS] = {0};
        uint8_t qntHouse = 0;
        uint8_t qntPlayers = 0;
    } Game;

    void Constructor(Game& game, int qntHouse, int qntPlayers){
        game.qntHouse = qntHouse;
        game.qntPlayers = qntPlayers;
    }

}

#endif