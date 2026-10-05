#pragma once

#include "raylib.h"
#include "utilities.hpp"
#include <string>
#include <cstdint>

using namespace std;

struct Menu{
    bool active = true;
    int option = 0;
};

struct MsgFeedback{
    string message = "";
    float time = 0.0f;
}; 

struct Config{
    int playerQnt = 0;
    string PATH = "";
    string playerNames[MAXPLAYERS];
};

struct Client{
    Config config;
    Menu menu;
    MsgFeedback feedback;
    bool pressedButton = false;
    bool gameOver = false;
    string winnerName = "";
    bool rolledThisTurn = false;
    int index = 0;
    int counter = 0;
};

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

struct Card {
    string desc = "";    // Descrição da Carta
    bool good;           // Carta Positiva ou negativa?
    uint8_t action;  // Tipo de ação
    uint8_t value = 0;   // Valor ($) da ação
    int8_t target = -1;  // Destino
    int8_t passBy = -1; 
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

struct EventDecision {
    uint8_t action = NONE;
    int8_t houseId = -1;
};

struct LiquidationDecision {
    bool active = false;
    int8_t playerId = -1;
    int8_t receiverId = -1;
    uint16_t amountOwed = 0;
    bool payEachPlayer = false;
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
};