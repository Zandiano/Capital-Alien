#include <cstdint>
#include "events.hpp"
#include "raylib.h"
#include "structs.hpp"
#include "constants.hpp"
#include "house.hpp"
#include "player.hpp"
#include "tabletop.hpp"
#include "game_renderer.hpp"
#include "filehandler.hpp"
#include <fstream>

uint32_t GetLiquidationValue(Game& game, Player& player) {
    uint32_t value = player.money;

    for(int i = 0; i < game.qntHouse; i++) {
        House& house = game.houses[i];

        if(house.owner == player.ID) {
            if(house.mortgaged) continue;

            value += house.mortgagePrice;
            value += house.housesBuilt * (house.residencePrice / 2);
        }
    }

    return value;
}

bool CanAfford(Game& game, Player& player, uint32_t value) {
    return GetLiquidationValue(game, player) >= value;
}

PAYMENT_STATUS VerifyMoney(Game& game, Player& player, uint32_t value) {
    if(player.money >= value) {
        return PAYMENT_STATUS::CAN_PAY;
    }
    if(CanAfford(game, player, value)) {
        return PAYMENT_STATUS::NEED_LIQUIDATION;
    }
    return PAYMENT_STATUS::BANKRUPT;
}

void ResolvePayment(Game& game) {
    LiquidationDecision& decision = game.liquidation;

    if(!decision.active) return;

    Player& player = GetPlayer(game, decision.playerId);

    if(decision.payEachPlayer) {
        uint32_t total = decision.amountOwed * (game.qntPlayers - 1);
        if(player.money >= total) {
            for(int i = 0; i < game.qntPlayers; i++) {
                if(i == player.ID) continue;
                TransferMoney(player, game.players[i], decision.amountOwed);
            }
            decision.active = false;
        }
    } else {
        if(player.money >= decision.amountOwed) {
            if(decision.receiverId == -1) {
                RemoveMoney(player, decision.amountOwed);
            } else {
                TransferMoney(player, game.players[decision.receiverId], decision.amountOwed);
            }
            decision.active = false;
        }
    }
}

bool SellHousesOrHotels(Game& game, Player& player, House& house, uint8_t qnt) {
    if(!house.housesBuilt || house.owner != player.ID || house.housesBuilt < qnt) {
        return false;
    }
    house.housesBuilt -= qnt;
    AddMoney(player, qnt * (house.residencePrice / 2));
    ResolvePayment(game);
    return true;
}

void Liquidate(Game& game, Player& from, uint8_t destID, uint32_t value, bool FLAG) {
    game.liquidation.active = true;
    game.liquidation.playerId = GetID(from);
    game.liquidation.receiverId = destID;
    game.liquidation.amountOwed = value;
    game.liquidation.payEachPlayer = FLAG;
}

void StartEvent(Player& player) {
    AddMoney(player, 200);
}

void GetChestCard(Game& game, Player& player) {
    Card card = game.chestCards[0];
    for(int i = 1; i < QNTCARDS; i++) {
        game.chestCards[i - 1] = game.chestCards[i];
    }
    game.chestCards[QNTCARDS - 1] = card;

    player.lastCard = card;

    switch(card.action) {
    case COLLECT_MONEY:
        AddMoney(player, card.value);
        break;
    case PAY_MONEY:
    {
        PAYMENT_STATUS status = VerifyMoney(game, player, card.value);
        if(status == CAN_PAY) {
            RemoveMoney(player, card.value);
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, card.value, false);
        } else {
            Bankrupt(player);
        }
        break;
    }
    case COLLECT_FROM_EACH_PLAYER:
        for(int i = 0; i < game.qntPlayers; i++) {
            if(game.players[i].ID == player.ID) continue;
            if(game.players[i].bankrupt) continue;
            PAYMENT_STATUS status = VerifyMoney(game, game.players[i], card.value);
            if(status == CAN_PAY) {
                TransferMoney(game.players[i], player, card.value);
            } else if(status == NEED_LIQUIDATION) {
                Liquidate(game, game.players[i], player.ID, card.value, false);
            } else {
                Bankrupt(game.players[i]);
            }
        }
        break;
    case GO_TO_JAIL:
        player.movedByCard = true;
        card.target = 10;
        player.arrested = true;
        break;
    case MOVE_AND_RECEIVE:
        player.movedByCard = true;
        AddMoney(player, card.value);
        break;
    case ESPECIAL_PAY:
    {
        uint32_t value = (game.housesBuilt) * 40 + (game.hotelsBuilt) * 115;
        PAYMENT_STATUS status = VerifyMoney(game, player, value);
        if(status == CAN_PAY) {
            RemoveMoney(player, value);
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, value, false);
        } else {
            Bankrupt(player);
        }
        break;
    }
    }
}

void GetChanceCard(Game& game, Player& player) {
    Card card = game.chanceCards[0];
    for(int i = 1; i < QNTCARDS; i++) {
        if(game.jailCardActive && i == QNTCARDS - 1) {
            game.chanceCards[i - 1] = card;
            break;
        }
        game.chanceCards[i - 1] = game.chanceCards[i];
    }
    if(!(card.action == GET_OUT_OF_JAIL) && !game.jailCardActive) {
        game.chanceCards[QNTCARDS - 1] = card;
    }

    player.lastCard = card;

    switch(card.action) {
    case MOVE_TO:
        player.movedByCard = true;
        break;
    case MOVE_AND_RECEIVE:
        player.movedByCard = true;
        AddMoney(player, card.value);
        break;
    case MOVE_RECEIVE_IF:
        if(player.houseIndex > card.target) {
            AddMoney(player, card.value);
        }
        player.movedByCard = true;
        break;
    case MOVE_NEAREST_RAILROAD:
        for(int i = 1; i <= game.qntHouse; i++) {
            int position = (GetPos(player) + i) % game.qntHouse;

            if(game.houses[position].type == RAILROAD) {
                player.movedByCard = true;
                card.target = position;
                break;
            }
        }
        break;
    case PAY_MONEY:
    {
        PAYMENT_STATUS status = VerifyMoney(game, player, card.value);
        if(status == CAN_PAY) {
            RemoveMoney(player, card.value);
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, card.value, false);
        } else {
            Bankrupt(player);
        }
        break;
    }
    case MOVE_NEAREST_COMPANY:
        for(int i = 1; i <= game.qntHouse; i++) {
            int position = (GetPos(player) + i) % game.qntHouse;

            if(game.houses[position].type == COMPANY) {
                player.movedByCard = true;
                card.target = position;
                break;
            }
        }
        break;
    case COLLECT_MONEY:
        AddMoney(player, card.value);
        break;
    case GET_OUT_OF_JAIL:
        player.jailCard = true;
        game.jailCardActive = true;
        break;
    case ESPECIAL_PAY:
    {
        uint32_t value = (game.housesBuilt) * 25 + (game.hotelsBuilt) * 100;
        PAYMENT_STATUS status = VerifyMoney(game, player, value);
        if(status == CAN_PAY) {
            RemoveMoney(player, value);
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, value, false);
        } else {
            Bankrupt(player);
        }
        break;
    }
    case PAY_EACH_PLAYER:
    {
        uint8_t activeP = 0;
        for(int i = 0; i < game.qntPlayers; i++) {
            if(game.players[i].bankrupt) continue;
            activeP++;
        }
        uint32_t value = card.value * (activeP - 1);
        PAYMENT_STATUS status = VerifyMoney(game, player, value);
        if(status == CAN_PAY) {
            for(int i = 0; i < game.qntPlayers; i++) {
                if(game.players[i].ID == player.ID) continue;
                if(game.players[i].bankrupt) continue;
                TransferMoney(player, game.players[i], card.value);
            }
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, value, true);
        } else {
            Bankrupt(player);
        }
        break;
    }
    }
}

void TaxesEvent(Game& game, Player& player) {
    if(GetPos(player) != 4) {
        PAYMENT_STATUS status = VerifyMoney(game, player, 75);

        switch(status) {
        case CAN_PAY:
            RemoveMoney(player, 75);
            break;

        case NEED_LIQUIDATION:
            Liquidate(game, player, -1, 75, false);
            break;

        default:
            Bankrupt(player);
            break;
        }
        return;
    }

    PAYMENT_STATUS status = VerifyMoney(game, player, 200);

    switch(status) {
    case CAN_PAY:
        RemoveMoney(player, 200);
        break;

    case NEED_LIQUIDATION:
        Liquidate(game, player, -1, 200, false);
        break;

    default:
        uint32_t total = GetLiquidationValue(game, player);

        if(total <= 0) {
            Bankrupt(player);
            break;
        }

        status = VerifyMoney(game, player, 0.1 * total);
        if(status == CAN_PAY) {
            RemoveMoney(player, 0.1 * total);
        } else if(status == NEED_LIQUIDATION) {
            Liquidate(game, player, -1, 0.1 * total, false);
        }
        break;
    }
}

void PrisionEvent(Player& player) {
    SetHouse(player, 10);
    player.arrested = true;
}

uint8_t GetPropCount(Game& game, Player& player, House& house) {
    int count = 0;
    switch(house.type) {
    case NORMAL:
        for(int i = 1; i < game.qntHouse; i++) {
            House& current = game.houses[i];
            if(current.type == NORMAL
                && ColorToInt(current.color) == ColorToInt(house.color)
                && current.owner == player.ID) {
                count++;
            }
        }
        return count;
        break;

    case COMPANY:
        if(game.houses[12].owner == player.ID) count++;
        if(game.houses[28].owner == player.ID) count++;
        return count;
        break;

    case RAILROAD:
        for(int i = 1; i < game.qntHouse; i++) {
            if(game.houses[i].type == RAILROAD && game.houses[i].owner == player.ID) count++;
        }
        return count;
        break;
    }
    return 0;
}

void CompanyEvent(Game& game, Player& player, House& house, uint8_t dice) {
    if(GetOwner(house) == GetID(player)) return;

    if(GetOwner(house) != -1) {
        Player& owner = GetPlayer(game, GetOwner(house));
        int mod = (GetPropCount(game, owner, house) == 2) ? 10 : 4;
        PAYMENT_STATUS status = VerifyMoney(game, player, mod * dice);

        switch(status) {
        case CAN_PAY:
            TransferMoney(player, owner, mod * dice);
            break;

        case NEED_LIQUIDATION:
            Liquidate(game, player, GetID(owner), mod * dice, false);
            break;

        default:
            Bankrupt(player);
        }
    } else {
        game.eventDecision.action = BUY;
        game.eventDecision.houseId = GetPos(player);
    }
}

void HouseEvent(Game& game, Player& player, House& house) {
    int id;
    
    if(GetOwner(house) == -1) {
        id = -1;
    } else {
        Player& owner = owner = GetPlayer(game, GetOwner(house));
        id = GetID(owner);
    }

    if(id == -1) {
        game.eventDecision.action = EVENT_ACTION::BUY;
        game.eventDecision.houseId = player.houseIndex;
        return;
    }

    if(id == GetID(player)) return;

    uint32_t rent = GetValue(game, house);
    PAYMENT_STATUS status = VerifyMoney(game, player, rent);

    switch(status) {
    case CAN_PAY:
        TransferMoney(player, game.players[house.owner], rent);
        break;

    case NEED_LIQUIDATION:
        Liquidate(game, player, id, rent, false);
        break;

    default:
        Bankrupt(player);
    }
}

void EventSelector(Game& game, House& house, Player& player, uint8_t dice) {
    switch(house.type) {
    case START:
        StartEvent(player);
        break;
    case NORMAL:
        HouseEvent(game, player, house);
        break;
    case RAILROAD:
        HouseEvent(game, player, house);
        break;
    case COMPANY:
        CompanyEvent(game, player, house, dice);
        break;
    case CHEST:
        GetChestCard(game, player);
        break;
    case QUESTION_MARK:
        GetChanceCard(game, player);
        break;
    case TAXES:
        TaxesEvent(game, player);
        break;
    case TELEPORT:
        PrisionEvent(player);
        break;
    }
}