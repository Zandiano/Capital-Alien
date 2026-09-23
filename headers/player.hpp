#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cstdint>
#include "structs.hpp"

using namespace std;

Player Constructor(uint8_t ID, const string& name, Color newColor);

uint8_t GetID(const Player& player);

string GetName(const Player& player);

uint32_t GetMoney(const Player& player);

Color GetPlayerColor(const Player& player);

void AddMoney(Player& player, uint32_t value);

void RemoveMoney(Player& player, uint32_t value);

void TransferMoney(Player& from, Player& dest, uint16_t value);

uint8_t GetPos(const Player& player);

bool NextHouse(Player& player, uint8_t maxHouses);

bool SetHouse(Player& player, uint8_t index);

#endif