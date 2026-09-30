#pragma once

// tudo do player fica aqui (por isso o inline), pode apagar o player.cpp

#include <string>
#include <fstream>
#include <cstdint>
#include "structs.hpp"

using namespace std;

const int MAX_NAME_SIZE = 16;

const Color DEFAULT_COLORS[] = {RED, BLUE, GREEN, ORANGE};


// ===== Validacao =====

inline string TrimName(const string& name){
    size_t start = name.find_first_not_of(' ');
    if(start == string::npos){
        return "";
    }
    size_t end = name.find_last_not_of(' ');
    return name.substr(start, end - start + 1);
}

inline bool IsNameValid(const string& name, string& error){
    string clean = TrimName(name);

    if(clean.empty()){
        error = "O nome nao pode ficar vazio.";
        return false;
    }
    if((int)clean.size() > MAX_NAME_SIZE){
        error = "O nome pode ter no maximo " + to_string(MAX_NAME_SIZE) + " letras.";
        return false;
    }
    for(size_t i = 0; i < clean.size(); i++){
        char c = clean[i];
        // aspas e barra quebram o json
        if(c == '"' || c == '\\' || (unsigned char)c < 32){
            error = "O nome tem um caractere que nao pode ser usado.";
            return false;
        }
    }
    return true;
}

inline bool SameColor(Color a, Color b){
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline bool SameName(const string& a, const string& b){
    if(a.size() != b.size()){
        return false;
    }
    for(size_t i = 0; i < a.size(); i++){
        char x = a[i];
        char y = b[i];
        if(x >= 'A' && x <= 'Z') x += 32;
        if(y >= 'A' && y <= 'Z') y += 32;
        if(x != y){
            return false;
        }
    }
    return true;
}

inline bool IsNameInUse(const Game& game, const string& name){
    string clean = TrimName(name);
    for(int i = 0; i < game.qntPlayers; i++){
        if(SameName(game.players[i].name, clean)){
            return true;
        }
    }
    return false;
}

inline bool IsColorInUse(const Game& game, Color color){
    for(int i = 0; i < game.qntPlayers; i++){
        if(SameColor(game.players[i].color, color)){
            return true;
        }
    }
    return false;
}


// ===== Dados basicos =====

inline Player Constructor(uint8_t ID, const string& name, Color newColor, uint32_t initialMoney){
    Player player{};
    string error;

    player.ID = ID;
    player.money = initialMoney;
    player.color = newColor;

    if(IsNameValid(name, error)){
        player.name = TrimName(name);
    } else {
        player.name = "Jogador " + to_string(ID + 1);
    }
    return player;
}

inline uint8_t GetID(const Player& player){
    return player.ID;
}

inline string GetName(const Player& player){
    return player.name;
}

inline uint32_t GetMoney(const Player& player){
    return player.money;
}

inline Color GetPlayerColor(const Player& player){
    return player.color;
}

inline uint8_t GetPos(const Player& player){
    return player.houseIndex;
}


// ===== Dinheiro =====

inline void AddMoney(Player& player, uint32_t value){
    player.money += value;
}

// se tirar mais do que tem o saldo dava a volta e virava bilhao
inline bool RemoveMoney(Player& player, uint32_t value){
    if(value > player.money){
        return false;
    }
    player.money -= value;
    return true;
}

inline bool TransferMoney(Player& from, Player& dest, uint32_t value){
    if(!RemoveMoney(from, value)){
        return false;
    }
    AddMoney(dest, value);
    return true;
}


// ===== Posicao =====

// true quando volta pro Inicio
inline bool NextHouse(Player& player, uint8_t maxHouses){
    if(maxHouses == 0){
        return false;
    }
    ++player.houseIndex %= maxHouses;
    return !player.houseIndex;
}

inline bool SetHouse(Player& player, uint8_t index){
    player.houseIndex = index;
    return !player.houseIndex;
}

inline bool SetHouse(Player& player, uint8_t index, uint8_t maxHouses){
    if(index >= maxHouses){
        return false;
    }
    player.houseIndex = index;
    return true;
}


// ===== Cadastro =====

inline void ResetPlayers(Game& game){
    game.qntPlayers = 0;
    game.playerIndex = 0;
}

inline bool CreatePlayer(Game& game, const string& name, Color color, uint32_t initialMoney, string& error){
    if(game.qntPlayers >= MAXPLAYERS){
        error = "Ja existem " + to_string(MAXPLAYERS) + " jogadores.";
        return false;
    }
    if(!IsNameValid(name, error)){
        return false;
    }

    string clean = TrimName(name);

    if(IsNameInUse(game, clean)){
        error = "Ja existe um jogador chamado " + clean + ".";
        return false;
    }
    if(IsColorInUse(game, color)){
        error = "Essa cor ja esta sendo usada por outro jogador.";
        return false;
    }

    uint8_t id = game.qntPlayers;
    game.players[id] = Constructor(id, clean, color, initialMoney);
    game.qntPlayers++;
    return true;
}

inline void CreateDefaultPlayers(Game& game, int qnt, uint32_t initialMoney){
    string error;
    int maxColors = array_size(DEFAULT_COLORS);

    ResetPlayers(game);
    for(int i = 0; i < qnt && i < maxColors; i++){
        CreatePlayer(game, "Jogador " + to_string(i + 1), DEFAULT_COLORS[i], initialMoney, error);
    }
}


// ===== Turno =====

inline uint8_t CountActivePlayers(const Game& game){
    uint8_t active = 0;
    for(int i = 0; i < game.qntPlayers; i++){
        if(!game.players[i].bankrupt){
            active++;
        }
    }
    return active;
}

inline bool SetTurn(Game& game, uint8_t index){
    if(index >= game.qntPlayers){
        return false;
    }
    if(game.players[index].bankrupt){
        return false;
    }
    game.playerIndex = index;
    return true;
}

// false se ninguem sobrou pra jogar
inline bool AdvanceTurn(Game& game){
    if(CountActivePlayers(game) == 0){
        return false;
    }

    int index = game.playerIndex;
    do {
        index = (index + 1) % game.qntPlayers;
        if(index == 0){
            game.round++;
        }
    } while(game.players[index].bankrupt);

    game.playerIndex = index;
    return true;
}


// ===== Propriedades =====

// o array list precisa ter MAXHOUSES posicoes
inline uint8_t GetProperties(const Game& game, const Player& player, uint8_t list[]){
    uint8_t count = 0;
    for(int i = 0; i < game.qntHouse; i++){
        if(game.houses[i].owner == player.ID){
            list[count] = i;
            count++;
        }
    }
    return count;
}

inline uint8_t UpdateTotalProperties(const Game& game, Player& player){
    uint8_t list[MAXHOUSES];
    uint8_t count = GetProperties(game, player, list);
    player.totalProperties = count;
    return count;
}

inline string PropertiesToString(const Game& game, const Player& player){
    uint8_t list[MAXHOUSES];
    uint8_t count = GetProperties(game, player, list);

    if(count == 0){
        return "Nenhuma propriedade";
    }

    string text = "";
    for(int i = 0; i < count; i++){
        if(i > 0){
            text += ", ";
        }
        text += game.houses[list[i]].name;
    }
    return text;
}


// ===== JSON =====
// um jogador por linha, o lastCard nao e salvo:
// {"id":0,"name":"Zandiano","money":2000,"color":[230,41,55,255],"position":0,"arrested":false,"jailCard":false,"bankrupt":false}

inline bool SavePlayersJSON(const Game& game, const string& filepath){
    ofstream file(filepath.c_str());
    if(!file.is_open()){
        return false;
    }

    file << "{\n";
    file << "\"currentPlayer\":" << (int)game.playerIndex << ",\n";
    file << "\"players\":[\n";

    for(int i = 0; i < game.qntPlayers; i++){
        const Player& p = game.players[i];

        file << "{";
        file << "\"id\":" << (int)p.ID << ",";
        file << "\"name\":\"" << p.name << "\",";
        file << "\"money\":" << p.money << ",";
        file << "\"color\":[" << (int)p.color.r << "," << (int)p.color.g << ","
             << (int)p.color.b << "," << (int)p.color.a << "],";
        file << "\"position\":" << (int)p.houseIndex << ",";
        file << "\"arrested\":" << (p.arrested ? "true" : "false") << ",";
        file << "\"jailCard\":" << (p.jailCard ? "true" : "false") << ",";
        file << "\"bankrupt\":" << (p.bankrupt ? "true" : "false");
        file << "}";

        if(i < game.qntPlayers - 1){
            file << ",";
        }
        file << "\n";
    }

    file << "]\n";
    file << "}\n";

    file.close();
    return true;
}

inline size_t FindKey(const string& line, const string& key){
    string search = "\"" + key + "\":";
    size_t pos = line.find(search);
    if(pos == string::npos){
        return string::npos;
    }
    return pos + search.size();
}

// le o numero e deixa o pos logo depois dele
inline bool ReadNumberAt(const string& line, size_t& pos, long long& value){
    size_t end = pos;
    while(end < line.size() && line[end] >= '0' && line[end] <= '9'){
        end++;
    }
    if(end == pos || end - pos > 10){
        return false;
    }
    value = stoll(line.substr(pos, end - pos));
    pos = end;
    return true;
}

inline bool ReadInt(const string& line, const string& key, long long& value){
    size_t pos = FindKey(line, key);
    if(pos == string::npos){
        return false;
    }
    return ReadNumberAt(line, pos, value);
}

inline bool ReadBool(const string& line, const string& key, bool& value){
    size_t pos = FindKey(line, key);
    if(pos == string::npos){
        return false;
    }
    if(line.compare(pos, 4, "true") == 0){
        value = true;
        return true;
    }
    if(line.compare(pos, 5, "false") == 0){
        value = false;
        return true;
    }
    return false;
}

inline bool ReadString(const string& line, const string& key, string& value){
    size_t pos = FindKey(line, key);
    if(pos == string::npos || pos >= line.size() || line[pos] != '"'){
        return false;
    }
    size_t end = line.find('"', pos + 1);
    if(end == string::npos){
        return false;
    }
    value = line.substr(pos + 1, end - pos - 1);
    return true;
}

inline bool ReadColor(const string& line, const string& key, Color& color){
    size_t pos = FindKey(line, key);
    if(pos == string::npos || pos >= line.size() || line[pos] != '['){
        return false;
    }
    pos++;

    long long parts[4];
    for(int i = 0; i < 4; i++){
        if(!ReadNumberAt(line, pos, parts[i]) || parts[i] > 255){
            return false;
        }
        char expected = (i < 3) ? ',' : ']';
        if(pos >= line.size() || line[pos] != expected){
            return false;
        }
        pos++;
    }

    color.r = (unsigned char)parts[0];
    color.g = (unsigned char)parts[1];
    color.b = (unsigned char)parts[2];
    color.a = (unsigned char)parts[3];
    return true;
}

// chamar depois do Init (usa qntHouse pra conferir a posicao).
// so mexe no jogo se o arquivo inteiro estiver certo
inline bool LoadPlayersJSON(Game& game, const string& filepath, string& error){
    ifstream file(filepath.c_str());
    if(!file.is_open()){
        error = "Nao foi possivel abrir " + filepath;
        return false;
    }

    Player loaded[MAXPLAYERS] = {};
    int count = 0;
    long long current = -1;
    string line;

    while(getline(file, line)){
        if(line.find("\"currentPlayer\":") != string::npos){
            if(!ReadInt(line, "currentPlayer", current)){
                error = "currentPlayer invalido.";
                return false;
            }
        } else if(line.find("\"id\":") != string::npos){
            if(count >= MAXPLAYERS){
                error = "Jogadores demais no arquivo (maximo " + to_string(MAXPLAYERS) + ").";
                return false;
            }

            Player p{};
            long long id, money, position;
            string name;

            bool ok = ReadInt(line, "id", id)
                   && ReadString(line, "name", name)
                   && ReadInt(line, "money", money)
                   && ReadColor(line, "color", p.color)
                   && ReadInt(line, "position", position)
                   && ReadBool(line, "arrested", p.arrested)
                   && ReadBool(line, "jailCard", p.jailCard)
                   && ReadBool(line, "bankrupt", p.bankrupt);

            if(!ok){
                error = "Jogador " + to_string(count + 1) + " esta incompleto ou mal escrito.";
                return false;
            }
            if(id != count){
                error = "Os ids dos jogadores devem ir de 0 em diante, em ordem.";
                return false;
            }
            if(money > 4294967295LL){
                error = "Saldo grande demais no jogador " + to_string(count + 1) + ".";
                return false;
            }
            if(position >= game.qntHouse){
                error = "O jogador " + to_string(count + 1) + " esta numa casa que nao existe.";
                return false;
            }
            if(!IsNameValid(name, error)){
                return false;
            }

            for(int i = 0; i < count; i++){
                if(SameName(loaded[i].name, name)){
                    error = "Nome repetido no arquivo: " + name + ".";
                    return false;
                }
                if(SameColor(loaded[i].color, p.color)){
                    error = "Cor repetida no arquivo.";
                    return false;
                }
            }

            p.ID = (uint8_t)id;
            p.name = TrimName(name);
            p.money = (uint32_t)money;
            p.houseIndex = (uint8_t)position;
            loaded[count] = p;
            count++;
        }
    }
    file.close();

    if(count == 0){
        error = "Nenhum jogador encontrado no arquivo.";
        return false;
    }
    if(current < 0 || current >= count){
        error = "currentPlayer ausente ou fora da lista de jogadores.";
        return false;
    }

    for(int i = 0; i < count; i++){
        game.players[i] = loaded[i];
    }
    game.qntPlayers = count;
    game.playerIndex = (uint8_t)current;

    for(int i = 0; i < count; i++){
        UpdateTotalProperties(game, game.players[i]);
    }
    return true;
}
