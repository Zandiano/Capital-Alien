#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "raylib.h"
#include "utilities.hpp"
#include <string>
#include <cstdint>

using namespace std;

struct House{
    string name = ""; // 4
    Color color = Color{255,0,0,255}; // 4
    uint16_t value = 0; // 2
    uint16_t price = 0; // 2
    uint8_t FLAGS = M0; // 1
    uint8_t tier = 0; // 1
    uint8_t type = NORMAL; // 1
    int8_t owner = -1; // 1
};

struct Player{
    uint8_t ID = 0;
    string name = "PLACEHOLDER";
    int32_t money = 0;
    Color color = RED;
    uint8_t houseIndex = 0;
};

struct Game{
    House houses[MAXHOUSES]; 
    Player players[MAXPLAYERS];
    uint8_t playerIndex = 0;
    uint8_t qntHouse = 0;
    uint8_t qntPlayers = 0;
    uint16_t round = 0;
};

#endif