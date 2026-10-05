#include "filehandler.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "utilities.hpp"

#define is_empty(file) file.peek() == EOF

using namespace std;

void ReadConfig(Config& config){
    ifstream file("src\\Data\\config.txt");
    cout << "Opening config" << endl;
    if(!file.is_open()) return;
    
    getline(file, config.PATH);
    cout << "Reading PATH -> " << config.PATH << endl;
    
    file >> config.playerQnt;
    file.get();
    cout << "Reading player quantity -> " << config.playerQnt << endl;

    string buffer;
    for(int i = 0; i < config.playerQnt; i++){
        getline(file, buffer);
        config.playerNames[i] = buffer + "\0";
        cout << "Reading player name(" << i << ") -> " << config.playerNames[i] << endl;
    }
    
    cout << "Closing config" << endl;
    file.close();
    WaitTime(0.2);
}

void ClearFile(const string& filepath){
    ofstream clear((filepath + "\\data.dat").c_str(), ios::trunc);
    cout << "Clearing data" << endl;
    clear.close();
}

void WriteCard(ofstream& file, const Card& card){
    file.write((const char*)&card.good, sizeof(bool));
    file.write((const char*)&card.action, sizeof(uint8_t));
    file.write((const char*)&card.value, sizeof(uint8_t));
    file.write((const char*)&card.target, sizeof(int8_t));
    file.write((const char*)&card.passBy, sizeof(int8_t));   
}

void ReadCard(ifstream& file, Card& card){
    file.read((char*)&card.good, sizeof(bool));
    file.read((char*)&card.action, sizeof(uint8_t));
    file.read((char*)&card.value, sizeof(uint8_t));
    file.read((char*)&card.target, sizeof(int8_t));
    file.read((char*)&card.passBy, sizeof(int8_t));   
}

void SendFile(const Game& game, const string& filepath){
    ClearFile(filepath);
    ofstream file((filepath + "\\data.dat").c_str(), ios::binary | ios::app);
    if(!file.is_open() || file.fail()){
        cout << "Error..." << endl;
        return;
    }

    cout << "Sending..." << endl;

    for(int i = 0; i < MAXHOUSES; i++){
        file.write((const char*)&game.houses[i].owner, sizeof(uint8_t));
        file.write((const char*)&game.houses[i].mortgaged, sizeof(bool));
        file.write((const char*)&game.houses[i].housesBuilt, sizeof(uint8_t));
    }
    
    for(int i = 0; i < MAXPLAYERS; i++){
        file.write((const char*)&game.players[i].ID, sizeof(uint8_t));
        file.write((const char*)&game.players[i].money, sizeof(uint32_t));
        file.write((const char*)&game.players[i].color, sizeof(Color));
        file.write((const char*)&game.players[i].houseIndex, sizeof(uint8_t));
        file.write((const char*)&game.players[i].arrested, sizeof(bool));
        file.write((const char*)&game.players[i].jailCard, sizeof(bool));
        file.write((const char*)&game.players[i].totalProperties, sizeof(uint32_t));
        file.write((const char*)&game.players[i].movedByCard, sizeof(bool));
        WriteCard(file, game.players[i].lastCard);
    }

    for(int i = 0; i < QNTCARDS; i++){
        WriteCard(file, game.chanceCards[i]);
    }

    for(int i = 0; i < QNTCARDS; i++){
        WriteCard(file, game.chestCards[i]);
    }

    file.write((const char*)&game.playerIndex, sizeof(uint8_t));
    file.write((const char*)&game.hotelsBuilt, sizeof(uint8_t));
    file.write((const char*)&game.housesBuilt, sizeof(uint8_t));
    file.write((const char*)&game.round, sizeof(uint16_t));
    file.write((const char*)&game.jailCardActive, sizeof(bool));

    file.write((const char*)&game.liquidation.active, sizeof(bool));
    file.write((const char*)&game.liquidation.playerId, sizeof(int8_t));
    file.write((const char*)&game.liquidation.receiverId, sizeof(int8_t));
    file.write((const char*)&game.liquidation.amountOwed, sizeof(uint16_t));
    file.write((const char*)&game.liquidation.payEachPlayer, sizeof(bool));

    file.write((const char*)&game.eventDecision.action, sizeof(uint8_t));
    file.write((const char*)&game.eventDecision.houseId, sizeof(int8_t));

    file.close();

    cout << "Sent." << endl;
}

void RetrieveFile(Game& game, const string& filepath){
    ifstream file((filepath + "\\data.dat").c_str(), ios::binary );
    if(!file.is_open() || file.fail() || is_empty(file)){
        cout << "Error..." << endl;
        return;
    }

    cout << "Retrieving..." << endl;
    
        for(int i = 0; i < MAXHOUSES; i++){
        file.read((char*)&game.houses[i].owner, sizeof(uint8_t));
        file.read((char*)&game.houses[i].mortgaged, sizeof(bool));
        file.read((char*)&game.houses[i].housesBuilt, sizeof(uint8_t));
    }
    
    for(int i = 0; i < MAXPLAYERS; i++){
        file.read((char*)&game.players[i].ID, sizeof(uint8_t));
        file.read((char*)&game.players[i].money, sizeof(uint32_t));
        file.read((char*)&game.players[i].color, sizeof(Color));
        file.read((char*)&game.players[i].houseIndex, sizeof(uint8_t));
        file.read((char*)&game.players[i].arrested, sizeof(bool));
        file.read((char*)&game.players[i].jailCard, sizeof(bool));
        file.read((char*)&game.players[i].totalProperties, sizeof(uint32_t));
        file.read((char*)&game.players[i].movedByCard, sizeof(bool));
        ReadCard(file, game.players[i].lastCard);
    }

    for(int i = 0; i < QNTCARDS; i++){
        ReadCard(file, game.chanceCards[i]);
    }

    for(int i = 0; i < QNTCARDS; i++){
        ReadCard(file, game.chestCards[i]);
    }

    file.read((char*)&game.playerIndex, sizeof(uint8_t));
    file.read((char*)&game.hotelsBuilt, sizeof(uint8_t));
    file.read((char*)&game.housesBuilt, sizeof(uint8_t));
    file.read((char*)&game.round, sizeof(uint16_t));
    file.read((char*)&game.jailCardActive, sizeof(bool));

    file.read((char*)&game.liquidation.active, sizeof(bool));
    file.read((char*)&game.liquidation.playerId, sizeof(int8_t));
    file.read((char*)&game.liquidation.receiverId, sizeof(int8_t));
    file.read((char*)&game.liquidation.amountOwed, sizeof(uint16_t));
    file.read((char*)&game.liquidation.payEachPlayer, sizeof(bool));

    file.read((char*)&game.eventDecision.action, sizeof(uint8_t));
    file.read((char*)&game.eventDecision.houseId, sizeof(int8_t));

    file.close();

    cout << "Retrieved." << endl;
}
