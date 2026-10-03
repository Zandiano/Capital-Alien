#include <iostream>
#include <ctime>
#include "game_logic.hpp"
#include "game_renderer.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include "constants.hpp"
#include "events.hpp"
#include "filehandler.hpp"

Game mainGame;
Client client;

string MoveAndTriggerEvent(Player& player, int dado1, int dado2, int total, const string& actorLabel) {
    uint8_t maxHouses = GetHouseQnt(mainGame);
    bool passedStart = false;

    for(int i = 0; i < total; i++) {
        if(NextHouse(player, maxHouses)) {
            passedStart = true;
        }
    }

    if(passedStart) {
        AddMoney(player, 200);
    }

    House& house = GetHouse(mainGame, GetPos(player));
    EventSelector(mainGame, house, player, (uint8_t)total);

    string msg = actorLabel + " tirou " + to_string(dado1) + " e " + to_string(dado2)
        + " (" + to_string(total) + ") e caiu em " + house.name;

    if(passedStart) {
        msg += ". Passou pelo Inicio e recebeu $200";
    }
    if(player.bankrupt) {
        msg += ". " + GetName(player) + " faliu!";
    }

    return msg;
}

void Init() {
    SetRandomSeed((unsigned int)time(NULL));

    ReadConfig(client.config);

    Init(mainGame, client.config);

    ClearFile(client.config.PATH);
}

void UpdatePre() {
    if(IsInMenu(client)) {
        int num = GetKeyPressed();
        if(num >= KEY_ONE && num <= KEY_FIVE)
            client.index = num - KEY_ONE;
    }
    if(client.pressedButton) {
        SendFile(mainGame, client.config.PATH);
    }
    RetrieveFile(mainGame, client.config.PATH);
}

void Update() {
}

void UpdatePost() {
}

void Render3D() {
    if(IsInMenu(client)) return;
    RenderHouse(mainGame);
}

void Render2D() {
    if(IsInMenu(client)) {
        RenderMenu(mainGame, client, client.config.PATH);
        return;
    }
    RenderRound(mainGame);
    RenderName(mainGame);
    TurnAlert(mainGame, client.index);
    RenderMoney(mainGame);
    client.pressedButton = RenderButtons(mainGame, client);
    RenderHouseInfo(mainGame);
    if(client.gameOver) {
        string texto = "Fim de jogo! Vencedor: " + client.winnerName;
        DrawText(texto.c_str(), 20, ScreenH / 2, 24, GOLD);
    }
}

void Debug() {
    std::cout << "Player: " << GetName(GetPlayer(mainGame)) << std::endl;
    std::cout << "House num: " << to_string(GetPos(GetPlayer(mainGame))) << std::endl;
    std::cout << "Client Index: " << client.index << std::endl;
}

string ActionRollDice() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    if(client.gameOver) {
        return "O jogo ja acabou! Vencedor: " + client.winnerName;
    }
    if(client.rolledThisTurn) {
        return "Voce ja jogou os dados nesta rodada! Passe a vez.";
    }

    Player& player = GetPlayer(mainGame);
    int dado1 = GetRandomValue(1, 6);
    int dado2 = GetRandomValue(1, 6);
    int total = dado1 + dado2;

    if(player.arrested) {
        bool saiu = false;
        string motivo;

        if(dado1 == dado2) {
            saiu = true;
            motivo = "tirou dados duplos";
        } else if(player.jailCard) {
            player.jailCard = false;
            saiu = true;
            motivo = "usou a carta de saida da prisao";
        } else if(player.money >= 50) {
            RemoveMoney(player, 50);
            saiu = true;
            motivo = "pagou $50 de fianca";
        }

        if(!saiu) {
            client.rolledThisTurn = true;
            return GetName(player) + " continua preso (tirou " + to_string(dado1) + " e " + to_string(dado2) + ")";
        }

        player.arrested = false;
        client.rolledThisTurn = true;
        return MoveAndTriggerEvent(player, dado1, dado2, total, GetName(player) + " " + motivo + " e saiu da prisao,");
    }

    client.rolledThisTurn = true;
    return MoveAndTriggerEvent(player, dado1, dado2, total, GetName(player));
}

string ActionBuy() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    if(mainGame.eventDecision.action != BUY) {
        return "Nao ha nada para comprar nesta casa.";
    }

    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, (uint8_t)mainGame.eventDecision.houseId);

    if(Buy(house, player)) {
        mainGame.eventDecision.action = NONE;
        mainGame.eventDecision.houseId = -1;
        return GetName(player) + " comprou " + house.name + " por $" + to_string(house.price);
    }

    return "Dinheiro insuficiente para comprar " + house.name;
}

string ActionBuild() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(BuildHouse(mainGame, house, player)) {
        string tipo = (house.housesBuilt >= 5) ? "um hotel" : "uma casa";
        return GetName(player) + " construiu " + tipo + " em " + house.name;
    }

    return "Nao e possivel construir em " + house.name + " agora.";
}

string ActionMortgage() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(MortgageProperty(mainGame, player, house)) {
        return GetName(player) + " hipotecou " + house.name + " e recebeu $" + to_string(house.mortgagePrice);
    }

    return "Nao e possivel hipotecar " + house.name + ".";
}

string ActionNegotiate() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    return "Negociacao entre jogadores ainda nao implementada.";
}

string ActionEndTurn() {
    if(client.index != GetID(GetPlayer(mainGame))) {
        return "Jogue no seu turno!";
    }
    if(client.gameOver) {
        return "O jogo ja acabou! Vencedor: " + client.winnerName;
    }

    if(!client.rolledThisTurn) {
        return "Role os dados!";
    }

    string finishedName = GetName(GetPlayer(mainGame));

    client.rolledThisTurn = false;
    mainGame.eventDecision.action = EVENT_ACTION::NONE;
    mainGame.eventDecision.houseId = -1;

    uint8_t alive = 0;
    int8_t lastAliveId = -1;
    for(int i = 0; i < mainGame.qntPlayers; i++) {
        if(!mainGame.players[i].bankrupt) {
            alive++;
            lastAliveId = i;
        }
    }

    if(alive <= 1) {
        client.gameOver = true;
        client.winnerName = (lastAliveId != -1) ? GetName(mainGame.players[lastAliveId]) : "Ninguem";
        return "Fim de jogo! " + client.winnerName + " venceu!";
    }

    uint8_t newIndex;
    do {
        newIndex = NextPlayer(mainGame);
        if(newIndex == 0) {
            NextRound(mainGame);
        }
    } while(mainGame.players[newIndex].bankrupt);

    return finishedName + " passou a vez. Agora e a vez de " + GetName(GetPlayer(mainGame)) + ".";
}
