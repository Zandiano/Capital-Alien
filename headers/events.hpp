#ifndef EVENTS_HPP
#define EVENTS_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "raylib.h"
#include "structs.hpp"


bool MortgageProperty(Game& game, Player& player, House& house);

bool SellHousesOrHotels(Game& game, Player& player, House& house, uint8_t qnt);

void eventSelector(Game& game, House& house, Player& player, uint8_t dice);

#endif