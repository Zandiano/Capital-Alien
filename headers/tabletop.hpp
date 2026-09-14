#ifndef TABLETOP_HPP
#define TABLETOP_HPP

#include "player.hpp"
#include "house.hpp"
#include "utilities.hpp"

using namespace PLR;
using namespace HSE;

namespace TBL{

    #define MAXHOUSES 255
    #define MAXPLAYERS 4

    typedef struct{
        House houses[MAXHOUSES]; 
        Player players[MAXPLAYERS];
        uint8_t playerIndex = 0;
        uint8_t qntHouse = 0;
        uint8_t qntPlayers = 0;
        uint16_t round = 0;
    } Game;

    void Init(Game& game, int qntHouse, int qntPlayers){
        game.qntHouse = qntHouse;
        game.qntPlayers = qntPlayers;
        
        for(int i = 0; i < qntHouse; i++){
            game.houses[i] = HSE::Constructor("House" + i, CA_Colors(i%array_size(all_colors)), 100, 10, M0, 0, HSE::NORMAL);
        }

        for(int i = 0; i < qntPlayers; i++){
            game.players[i] = PLR::Constructor(" Player" + i, CA_Colors(i%array_size(all_colors)));
        }
    }

    PLR::Player* GetPlayer(Game& game){
        return &game.players[game.playerIndex]; 
    }
    
    PLR::Player* GetPlayer(Game& game, uint8_t index){
        return &game.players[index];
    }
    
    HSE::House* GetHouse(Game& game, uint8_t index){
        return &game.houses[index];
    }
    
    uint8_t GetPlayerQnt(const Game& game){
        return game.qntPlayers;
    }

    uint8_t GetHouseQnt(const Game& game){
        return game.qntHouse;
    }

    uint16_t GetRound(const Game& game){
        return game.round;
    }

    void NextRound(Game& game){
        NextHouse(*GetPlayer(game), GetHouseQnt(game));
        game.round++;
    }

    bool InitPlayer(Game& game, Player newPlayer){
        *GetPlayer(game) = newPlayer;
        return true;
    }

    uint8_t NextPlayer(Game& game){
        ++game.playerIndex %= GetPlayerQnt(game);
        return game.playerIndex;
    }

    bool SetPlayer(Game& game, uint8_t index){
        if(index > GetPlayerQnt(game)){
            return false;
        }
        game.playerIndex = index;
        return true;
    }
}

#endif