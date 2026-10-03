#include "structs.hpp"
#include "house.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "tabletop.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

void generateCards(Card cards_arr[], int FLAG);

Color StringToColorName(string colorName) {
    if(colorName == "YELLOW")     return YELLOW;
    if(colorName == "ORANGE")     return ORANGE;
    if(colorName == "PURPLE")     return PURPLE;
    if(colorName == "GREEN")      return GREEN;
    if(colorName == "PINK")       return PINK;
    if(colorName == "BLUE")       return BLUE;
    if(colorName == "GRAY")       return GRAY;
    if(colorName == "RED")        return RED;

    return WHITE;
}

HSETYPE StringToEnum1(string enumName) {
    if(enumName == "QUESTION_MARK") return HSETYPE::QUESTION_MARK;
    if(enumName == "TELEPORT") return HSETYPE::TELEPORT;
    if(enumName == "RAILROAD") return HSETYPE::RAILROAD;
    if(enumName == "PRISION") return HSETYPE::PRISION;
    if(enumName == "COMPANY") return HSETYPE::COMPANY;
    if(enumName == "NORMAL") return HSETYPE::NORMAL;
    if(enumName == "START") return HSETYPE::START;
    if(enumName == "TAXES") return HSETYPE::TAXES;
    if(enumName == "CHEST") return HSETYPE::CHEST;
    if(enumName == "PARK") return HSETYPE::PARK;

    return HSETYPE::NORMAL;
}

CARD_ACTION StringToEnum2(string enumName) {
    if(enumName == "COLLECT_MONEY") return CARD_ACTION::COLLECT_MONEY;
    if(enumName == "PAY_MONEY") return CARD_ACTION::PAY_MONEY;
    if(enumName == "ESPECIAL_PAY") return CARD_ACTION::ESPECIAL_PAY;
    if(enumName == "MOVE_TO") return CARD_ACTION::MOVE_TO;
    if(enumName == "MOVE_BACK") return CARD_ACTION::MOVE_BACK;
    if(enumName == "MOVE_AND_RECEIVE") return CARD_ACTION::MOVE_AND_RECEIVE;
    if(enumName == "MOVE_NEAREST_RAILROAD") return CARD_ACTION::MOVE_NEAREST_RAILROAD;
    if(enumName == "MOVE_NEAREST_COMPANY") return CARD_ACTION::MOVE_NEAREST_COMPANY;
    if(enumName == "GO_TO_JAIL") return CARD_ACTION::GO_TO_JAIL;
    if(enumName == "GET_OUT_OF_JAIL") return CARD_ACTION::GET_OUT_OF_JAIL;
    if(enumName == "PAY_EACH_PLAYER") return CARD_ACTION::PAY_EACH_PLAYER;
    if(enumName == "COLLECT_FROM_EACH_PLAYER") return CARD_ACTION::COLLECT_FROM_EACH_PLAYER;
    if(enumName == "MOVE_RECEIVE_IF") return CARD_ACTION::MOVE_RECEIVE_IF;

    return CARD_ACTION::MOVE_TO;
}

House readFile(string line) {
    stringstream ss(line);
    House newHouse;
    string str;

    getline(ss, newHouse.name, ',');
    getline(ss, str, ',');
    newHouse.color = StringToColorName(str);
    getline(ss, str, ',');
    newHouse.type = StringToEnum1(str);
    getline(ss, str, ',');
    newHouse.value = stoi(str);
    getline(ss, str, ',');
    newHouse.price = stoi(str);
    getline(ss, str, ',');
    newHouse.residencePrice = stoi(str);
    getline(ss, str, ';');
    newHouse.mortgagePrice = stoi(str);

    return newHouse;
}

void Init(Game& game, const Config& config) {
    game.qntHouse = MAXHOUSES;
    game.qntPlayers = config.playerQnt;

    generateCards(game.chanceCards, 0);
    generateCards(game.chestCards, 1);

    ifstream fileBoard("src\\Data\\boardData.txt");
    string line;
    int i = 0;
    if(fileBoard.is_open()) {
        while(getline(fileBoard, line)) {
            game.houses[i++] = readFile(line);
        }
        i = 0;
    } else {
        cout << "Erro ao abrir arquivo do tabuleiro!";
        return;
    }
    fileBoard.close();

    Color defaultColors[MAXPLAYERS] = { RED, BLUE, GREEN, ORANGE, BROWN };

    for(int i = 0; i < game.qntPlayers; i++) {
        game.players[i] = Constructor(i, config.playerNames[i], defaultColors[i % MAXPLAYERS], INITMONEY);
    }
}

Player& GetPlayer(Game& game) {
    return game.players[game.playerIndex];
}

Player& GetPlayer(Game& game, uint8_t index) {
    return game.players[index];
}

House& GetHouse(Game& game, uint8_t index) {
    return game.houses[index];
}

uint8_t GetPlayerQnt(const Game& game) {
    return game.qntPlayers;
}

uint8_t GetHouseQnt(const Game& game) {
    return game.qntHouse;
}

uint16_t GetRound(const Game& game) {
    return game.round;
}

void NextRound(Game& game) {
    game.round++;
}

uint8_t NextPlayer(Game& game) {
    do {
        ++game.playerIndex %= GetPlayerQnt(game);
    } while(GetPlayer(game).bankrupt);
    return game.playerIndex;
}

Card readFileCards(string line) {
    stringstream ss(line);
    Card newCard;
    string str;

    getline(ss, newCard.desc, ',');
    getline(ss, str, ',');
    istringstream(str) >> boolalpha >> newCard.good;
    getline(ss, str, ',');
    newCard.action = StringToEnum2(str);
    getline(ss, str, ',');
    newCard.value = stoi(str);
    getline(ss, str, ',');
    newCard.target = stoi(str);
    getline(ss, str, ';');
    newCard.passBy = stoi(str);

    return newCard;
}

void generateCards(Card cards_arr[], int FLAG) {
    std::random_device rd;

    std::mt19937 gen(rd());

    if(FLAG) {
        ifstream fileCards("src\\Data\\cardsChestData.txt");
        string line;
        int i = 0;
        if(fileCards.is_open()) {
            while(getline(fileCards, line) && i < QNTCARDS) {
                cards_arr[i++] = readFileCards(line);
            }
            i = 0;
        } else {
            cout << "Erro ao abrir arquivo das cartas!";
            return;
        }
        fileCards.close();
    } else {

        ifstream fileCards("src\\Data\\cardsChanceData.txt");
        string line;
        int i = 0;

        if(fileCards.is_open()) {
            while(getline(fileCards, line) && i < QNTCARDS) {
                cards_arr[i++] = readFileCards(line);
            }
            i = 0;
        } else {
            cout << "Erro ao abrir arquivo das cartas!";
            return;
        }
        fileCards.close();
    }
    std::shuffle(cards_arr, cards_arr + QNTCARDS, gen);
}