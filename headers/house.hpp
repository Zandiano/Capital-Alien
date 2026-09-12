#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "player.hpp"
#include "raylib.h"
#include "utilities.hpp"

using namespace PLR;
using namespace std;

namespace HSE{

    #define PARK M1
    #define START M2
    #define QUESTION_MARK M3
    #define PRISION M4
    #define TELEPORT M5

    typedef struct{
        string name = "";
        Player* owner = nullptr;
        uint16_t value = 0;
        uint8_t FLAGS = M0; // pensando em mudar de flags pra um enum e deixar as flags para modificadores das casas (ex: predio, grama verde, perigoso)
        uint8_t tier = 0;
    } House;

    House Constructor(const string& name, uint16_t value, uint8_t FLAGS, uint8_t tier){
        if(FLAGS & START){
            return {name: "Start", value: value, FLAGS: START};
        }
        return {name: name, value: value, FLAGS: FLAGS, tier: tier};
    }

    bool SetOwner(House& house, Player* newOwner){
        Player* lastOwner = house.owner;
        if(!(house.FLAGS & START)){
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