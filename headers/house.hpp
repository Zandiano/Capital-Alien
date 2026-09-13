#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "player.hpp"
#include "raylib.h"
#include "utilities.hpp"

using namespace PLR;
using namespace std;

namespace HSE{

    typedef enum{
        NORMAL,
        START,
        PARK,
        QUESTION_MARK,
        PRISION,
        TELEPORT
    } HSETYPE;

    typedef struct{
        string name = "";
        Player* owner = nullptr;
        uint16_t value = 0;
        uint8_t FLAGS = M0;
        uint8_t tier = 0;
        HSETYPE type = NORMAL;
    } House;

    House Constructor(const string& name, uint16_t value, uint8_t FLAGS, uint8_t tier, HSETYPE type){
        return {name: name, value: value, FLAGS: FLAGS, tier: tier, type: type};
    }

    bool SetOwner(House& house, Player* newOwner){
        Player* lastOwner = house.owner;
        if(house.type == NORMAL){
            house.owner = newOwner;
        }
        return lastOwner != house.owner;
    }

    Player* GetOwner(const House& house){
        return house.owner;
    }

    uint16_t PayRent(House house, Player& payer){
        TransferMoney(payer, *house.owner, house.value);
        return house.value;
    }
}

#endif