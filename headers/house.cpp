#include <string>
#include "raylib.h"
#include "structs.hpp"
#include "tabletop.hpp"
#include "player.hpp"


House Constructor(const string& name, Color color, uint8_t value, uint8_t price, uint8_t FLAGS, uint8_t tier, HSETYPE type){
    return House{name, color, value, price, FLAGS, tier, (uint8_t)type, -1};
}

bool SetOwner(House& house, int8_t newOwner){
    int8_t lastOwner = house.owner;
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

int8_t GetOwner(const House& house){
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
    SetOwner(house, GetID(player));
    return true;
}

uint16_t PayRent(Game& game, const House& house, Player& payer){
    TransferMoney(payer, GetPlayer(game, uint8_t(GetOwner(house))), house.value);
    return house.value;
}