#include "raylib.h"
#include "structs.hpp"
#include "constants.hpp"
#include "house.hpp"
#include "player.hpp"

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

    Player& player = game.players[decision.playerId];

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
                RemoveMoney(
                    player,
                    decision.amountOwed
                );
            } else {
                TransferMoney(player, game.players[decision.receiverId], decision.amountOwed);
            }
            decision.active = false;
        }
    }
}

/* Hipoteca um imóvel em posse do jogador selecionado */
bool MortgageProperty(Game& game, Player& player, House& house) {
    if(house.owner != player.ID)
        return false;
    if(house.mortgaged)
        return false;
    if(house.housesBuilt != 0)
        return false;

    AddMoney(player, house.mortgagePrice);
    house.mortgaged = true;
    ResolvePayment(game);

    return true;
}

/* Vende Casas/Hotéis dentro de um imóvel em posse do jogador selecionado */
bool SellHousesOrHotels(Game& game, Player& player, House& house, uint8_t qnt) {
    if(!house.housesBuilt || house.owner != player.ID || house.housesBuilt < qnt) {
        return false;
    }
    house.housesBuilt -= qnt;
    AddMoney(player, qnt * (house.residencePrice / 2));
    ResolvePayment(game);
    return true;
}

void goesBankrupt(Player& player) {
    player.bankrupt = true;
}

void activateLiquidation(Game& game, Player& player, int8_t id, uint32_t value, bool FLAG) {
    game.liquidation.active = true;
    game.liquidation.playerId = player.ID;
    game.liquidation.receiverId = id;
    game.liquidation.amountOwed = value;
    game.liquidation.payEachPlayer = FLAG;
}

void startEvent(Player& player) {
    AddMoney(player, 200);
}

void getChestCard(Game& game, Player& player) {
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
            activateLiquidation(game, player, -1, card.value, false);
        } else {
            goesBankrupt(player);
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
                activateLiquidation(game, game.players[i], player.ID, card.value, false);
            } else {
                goesBankrupt(game.players[i]);
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
            activateLiquidation(game, player, -1, value, false);
        } else {
            goesBankrupt(player);
        }
        break;
    }
    }
}

void getChanceCard(Game& game, Player& player) {
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
            activateLiquidation(game, player, -1, card.value, false);
        } else {
            goesBankrupt(player);
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
            activateLiquidation(game, player, -1, value, false);
        } else {
            goesBankrupt(player);
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
            activateLiquidation(game, player, -1, value, true);
        } else {
            goesBankrupt(player);
        }
        break;
    }
    }
}

void taxesEvent(Game& game, Player& player) {
    if(player.houseIndex == 4) {
        PAYMENT_STATUS status = VerifyMoney(game, player, 2010);
        if(status == CAN_PAY) {
            RemoveMoney(player, 200);
        } else if(status == NEED_LIQUIDATION) {
            activateLiquidation(game, player, -1, 200, false);
        } else {
            uint32_t total = GetLiquidationValue(game, player);
            if(total > 0) {
                status = VerifyMoney(game, player, 0.1 * total);
                if(status == CAN_PAY) {
                    RemoveMoney(player, 0.1 * total);
                } else if(status == NEED_LIQUIDATION) {
                    activateLiquidation(game, player, -1, 0.1 * total, false);
                }
            } else {
                goesBankrupt(player);
            }
        }
    } else {
        PAYMENT_STATUS status = VerifyMoney(game, player, 75);
        if(status == CAN_PAY) {
            RemoveMoney(player, 75);
        } else if(status == NEED_LIQUIDATION) {
            activateLiquidation(game, player, -1, 75, false);
        } else {
            goesBankrupt(player);
        }
    }
}

void teleportEvent(Player& player) {
    SetHouse(player, 10);
    player.arrested = true;
}

uint8_t ownerOfHowMany(Game& game, Player& player, House& house) {
    int count = 0;
    if(house.type == COMPANY) {
        if(game.houses[12].owner == player.ID) count++;
        if(game.houses[28].owner == player.ID) count++;
        return count;
    }
    if(house.type == RAILROAD) {
        for(int i = 1; i < game.qntHouse; i++) {
            if(game.houses[i].type == RAILROAD && game.houses[i].owner == player.ID) count++;  
        }
        return count;
    }
    if(house.type == NORMAL) {
        for(int i = 1; i < game.qntHouse; i++) {
            House& current = game.houses[i];
            if(current.type == NORMAL 
                && ColorToInt(current.color) == ColorToInt(house.color) 
                && current.owner == player.ID) { count++; }
        }
        return count;
    }
}

void auctionEvent(Game& game, Player& player, House& house) {
    game.eventDecision.action = EVENT_ACTION::AUCTION;
    game.eventDecision.houseId = player.houseIndex;
    game.auction.active = true;
    game.auction.houseId = player.houseIndex;
    game.auction.currentBid = 0;
    game.auction.highestBidder = -1;
    for(int i = 0; i < game.qntPlayers; i++) {
        if(!game.players[i].bankrupt) {
            game.auction.currentPlayer = i;
            break;
        }
    }
}

void companyEvent(Game& game, Player& player, House& house, uint8_t dice) {
    if(house.owner == player.ID) {
        return;
    }
    if(house.owner != -1) {
        if(ownerOfHowMany(game, game.players[house.owner], house) == 2) {
            PAYMENT_STATUS status = VerifyMoney(game, player, 10 * dice);
            if(status == CAN_PAY) {
               TransferMoney(player, game.players[house.owner], 10 * dice); 
            } else if(status == NEED_LIQUIDATION) {
                activateLiquidation(game, player, house.owner, 10 * dice, false);
            } else {
                goesBankrupt(player);
            }
        } else {
            PAYMENT_STATUS status = VerifyMoney(game, player, 4 * dice);
            if(status == CAN_PAY) {
               TransferMoney(player, game.players[house.owner], 4 * dice); 
            } else if(status == NEED_LIQUIDATION) {
                activateLiquidation(game, player, house.owner, 4 * dice, false);
            } else {
                goesBankrupt(player);
            }
        }
    } else {
        game.eventDecision.action = BUY;
        game.eventDecision.houseId = player.houseIndex;
    }
}

uint32_t rentValue(Game& game, Player& player, House& house) {
    uint32_t value = 0;
    if(house.type == NORMAL) {
        switch(house.housesBuilt) {
        case 0:
            value = house.value;
            break;
        case 1:
            value = house.value * 5;
            break;
        case 2:
            value = house.value * 15;
            break;
        case 3:
            value = house.value * 40;
            break;
        case 4:
            value = house.value * 60;
            break;
        case 5:
            value = house.value * 80;
            break;
        }
    } else if(house.type == RAILROAD) {
        uint8_t count = 0;
        for(int i = 5; i < 36; i += 10) {
            if(game.houses[i].owner == player.ID) count++;
        }
        switch(count) {
        case 1:
            value = 25;
            break;
        case 2:
            value = 50;
            break;
        case 3:
            value = 100;
            break;
        case 4:
            value = 200;
            break;
        }
    }
    return value;
}

void normalHouseEvent(Game& game, Player& player, House& house) {
    if (house.owner == -1) {
        game.eventDecision.action = EVENT_ACTION::BUY;
        game.eventDecision.houseId = player.houseIndex;
        return;
    }

    if (house.owner == player.ID) {
        return;
    }

    uint32_t rent = rentValue(game, player, house);
    PAYMENT_STATUS status = VerifyMoney(game, player, rent);
    if(status == CAN_PAY) {
        TransferMoney(player, game.players[house.owner], rent);
    } else if(status == NEED_LIQUIDATION) {
        activateLiquidation(game, player, house.owner, rent, false);
    } else {
        goesBankrupt(player);
    }
}

void railRoadEvent(Game& game, Player& player, House& house) {
    if (house.owner == -1) {
        game.eventDecision.action = EVENT_ACTION::BUY;
        game.eventDecision.houseId = player.houseIndex;
        return;
    }

    if (house.owner == player.ID) {
        return;
    }

    uint32_t rent = rentValue(game, player, house);
    PAYMENT_STATUS status = VerifyMoney(game, player, rent);
    if(status == CAN_PAY) {
        TransferMoney(player, game.players[house.owner], rent);
    } else if(status == NEED_LIQUIDATION) {
        activateLiquidation(game, player, house.owner, rent, false);
    } else {
        goesBankrupt(player);
    }
}

void eventSelector(Game& game, House& house, Player& player, uint8_t dice) {
    switch(house.type) {
    case START:
        startEvent(player);
        break;
    case NORMAL:
        normalHouseEvent(game, player, house);
        break;
    case RAILROAD:
        railRoadEvent(game, player, house);
        break;
    case COMPANY:
        companyEvent(game, player, house, dice);
        break;
    case CHEST:
        getChestCard(game, player);
        break;
    case QUESTION_MARK:
        getChanceCard(game, player);
        break;
    case TAXES:
        taxesEvent(game, player);
        break;
    case TELEPORT:
        teleportEvent(player);
        break;
    }
}