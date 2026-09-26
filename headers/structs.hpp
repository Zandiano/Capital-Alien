#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "raylib.h"
#include "utilities.hpp"
#include <string>
#include <cstdint>

using namespace std;

struct House{
    string name = "";           // Nome
    Color color;                // Cor
    uint8_t type;               // Tipo da casa
    uint16_t value;             // Valor do aluguel base
    uint16_t price;             // Valor para comprar a casa
    uint16_t residencePrice;    // Valor para comprar uma residência
    uint16_t mortgagePrice = 0; // Valor da hipoteca
    int8_t owner = -1;          // Dono
    bool mortgaged = false;     // Está Hipotecada?
    uint8_t housesBuilt = 0;    // Residências construídas
};

struct Player{
    uint8_t ID = 0;
    string name = "PLACEHOLDER";
    uint32_t money = 0;
    Color color = RED;
    uint8_t houseIndex = 0;
    bool arrested = false;
    bool jailCard = false;
    uint32_t totalProperties = 0;
    bool bankrupt = false;
    bool movedByCard = false;
    Card lastCard;
};

struct Game{
    House houses[MAXHOUSES]; 
    Player players[MAXPLAYERS];
    Card chanceCards[QNTCARDS];
    Card chestCards[QNTCARDS];
    uint8_t playerIndex = 0;
    uint8_t qntHouse = 0;
    uint8_t qntPlayers = 0;
    uint8_t hotelsBuilt = 0;
    uint8_t housesBuilt = 0;
    uint16_t round = 0;
    LiquidationDecision liquidation;
    bool jailCardActive = false;
    EventDecision eventDecision;
    Auction auction;
};

struct Card {
    string desc = "";    // Descrição da Carta
    bool good;           // Carta Positiva ou negativa?
    CARD_ACTION action;  // Tipo de ação
    uint8_t value = 0;   // Valor ($) da ação
    int8_t target = -1;  // Destino
    int8_t passBy = -1; 
};

struct LiquidationDecision {
    bool active = false;
    int8_t playerId = -1;
    int8_t receiverId = -1;
    uint16_t amountOwed = 0;
    bool payEachPlayer = false;
};

struct EventDecision {
    EVENT_ACTION action = NONE;
    int houseId = -1;
};

struct Auction {
    bool active = false;
    int houseId = -1;
    int currentPlayer = -1;
    uint32_t currentBid = 0;
    int highestBidder = -1;
};

#endif