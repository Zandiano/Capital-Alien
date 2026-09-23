#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <math.h>
#include "utilities.hpp"

using namespace std;

struct Player{
    string name = "PLACEHOLDER";
    int32_t money = 0;
    Color color = RED;
    uint8_t houseIndex = 0;
};

Player Constructor(const string& name, Color color){
    return {name: name, color: color};
}

string GetName(const Player& player){
    return player.name;
}

uint32_t GetMoney(const Player& player){
    return player.money;
}

Color GetPlayerColor(const Player& player){
    return player.color;
}

void AddMoney(Player& player, uint32_t value){
    player.money += value;
}

void RemoveMoney(Player& player, uint32_t value){
    player.money -= value;
}

void TransferMoney(Player& from, Player& dest, uint16_t value){
    RemoveMoney(from, value);
    AddMoney(dest, value);
}

uint8_t GetPos(const Player& player){
    return player.houseIndex;
}

bool NextHouse(Player& player, uint8_t maxHouses){
    ++player.houseIndex %= maxHouses;
    return !player.houseIndex;
}

bool SetHouse(Player& player, uint8_t index){
    player.houseIndex = index;
    return !player.houseIndex;
}

#endif