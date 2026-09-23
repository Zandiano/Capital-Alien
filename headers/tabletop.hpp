#ifndef TABLETOP_HPP
#define TABLETOP_HPP

#include "player.hpp"
#include "house.hpp"
#include "utilities.hpp"


#define MAXHOUSES 255
#define MAXPLAYERS 4

struct Game{
    House houses[MAXHOUSES]; 
    Player players[MAXPLAYERS];
    uint8_t playerIndex = 0;
    uint8_t qntHouse = 0;
    uint8_t qntPlayers = 0;
    uint16_t round = 0;
};

void Init(Game& game, int qntHouse, int qntPlayers){
    game.qntHouse = qntHouse;
    game.qntPlayers = qntPlayers;
    
    for(int i = 0; i < qntHouse; i++){
        game.houses[i] = Constructor("House" + i, RED, 100, 10, M0, 0, NORMAL);
    }

    for(int i = 0; i < qntPlayers; i++){
        game.players[i] = Constructor(" Player" + i, RED);
    }
}

Player& GetPlayer(Game& game){
    return game.players[game.playerIndex]; 
}

Player& GetPlayer(Game& game, uint8_t index){
    return game.players[index];
}

House& GetHouse(Game& game, uint8_t index){
    return game.houses[index];
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
    NextHouse(GetPlayer(game), GetHouseQnt(game));
    game.round++;
}

bool InitPlayer(Game& game, Player newPlayer){
    GetPlayer(game) = newPlayer;
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

#endif