#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "raylib.h"
#include "structs.hpp"

House Constructor(const string& name, Color color, uint8_t value, uint8_t price, uint8_t FLAGS, uint8_t tier, HSETYPE type);

bool SetOwner(House& house, int8_t newOwner);

void SetHouseColor(House& house, Color color);

Color GetHouseColor(const House& house);

int8_t GetOwner(const House& house);

bool Buy(House& house, Player& player);

uint16_t PayRent(Game& game, const House& house, Player& payer);

#endif