#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "player.hpp"
#include "raylib.h"

using namespace PLR;
using namespace std;

namespace HSE{
    typedef struct{
        string name = "";
        Player* owner = nullptr;
        uint16_t rent = 0;
    } House;

    House Constructor(string name, uint16_t rent){
        return {name: name, rent: rent};
    }
}

#endif