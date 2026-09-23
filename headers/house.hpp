#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "player.hpp"
#include "raylib.h"
#include "utilities.hpp"

typedef enum{
    NORMAL,
    START,
    PARK,
    QUESTION_MARK,
    PRISION,
    TELEPORT
} HSETYPE;

typedef struct{
    string name = ""; // 4
    Player* owner = nullptr; // 4
    Color color = Color{255,0,0,255}; // 4
    uint16_t value = 0; // 2
    uint16_t price = 0; // 2
    uint8_t FLAGS = M0; // 1
    uint8_t tier = 0; // 1
    uint8_t type = NORMAL; // 1
} House;

House Constructor(const string& name, Color color, uint8_t value, uint8_t price, uint8_t FLAGS, uint8_t tier, HSETYPE type){
    return {name: name, color: color, value: value, price: price, FLAGS: FLAGS, tier: tier, type: type};
}

bool SetOwner(House& house, Player* newOwner){
    Player* lastOwner = house.owner;
    if(house.type == NORMAL){
        house.owner = newOwner;
    }
    return lastOwner != house.owner;
}

void SetHouseColor(House& house, Color color){
    house.color = color;
}

Color GetHouseColor(const House& house){
    return house.color;
}

Player* GetOwner(const House& house){
    return house.owner;
}

bool Buy(House& house, Player& player){
    if(house.owner){
        //ToDo
    }
    else if(player.money < house.price || !house.price){
        return false;
    }

    RemoveMoney(player, house.price);
    SetOwner(house, &player);
    return true;
}

uint16_t PayRent(House house, Player& payer){
    TransferMoney(payer, *house.owner, house.value);
    return house.value;
}

#endif