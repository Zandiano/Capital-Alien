#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <math.h>

using namespace std;

namespace PLR{
    typedef struct{
        string name = "";
        int32_t money = 0;
        uint8_t houseIndex = 0;
    } Player;

    Player Constructor(const string& name){
        return {name: name};
    }

    uint32_t GetMoney(const Player& player){
        return player.money;
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

    bool NextHouse(Player& player){
        player.houseIndex++;
        return !player.houseIndex;
    }

    bool SetHouse(Player& player, uint8_t index){
        player.houseIndex = index;
        return !player.houseIndex;
    }
}

#endif