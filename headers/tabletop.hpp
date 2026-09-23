#ifndef TABLETOP_HPP
#define TABLETOP_HPP

#include "structs.hpp"

void Init(Game& game, int qntHouse, int qntPlayers);

Player& GetPlayer(Game& game);

Player& GetPlayer(Game& game, uint8_t index);

House& GetHouse(Game& game, uint8_t index);

uint8_t GetPlayerQnt(const Game& game);

uint8_t GetHouseQnt(const Game& game);

uint16_t GetRound(const Game& game);

void NextRound(Game& game);

bool InitPlayer(Game& game, Player newPlayer);

uint8_t NextPlayer(Game& game);

bool SetPlayer(Game& game, uint8_t index);

#endif