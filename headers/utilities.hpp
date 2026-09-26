#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

#define array_size(arr) int(sizeof((arr))/sizeof((arr)[0]))

#define MAXHOUSES 255
#define MAXPLAYERS 4
#define QNTCARDS 16

typedef enum{
    M0 = 0,
    M1 = 1,
    M2 = 2,
    M3 = 4,
    M4 = 8,
    M5 = 16,
    M6 = 32,
    M7 = 64,
    M8 = 128,
} BITMASK;

typedef enum{
    NORMAL,
    START,
    PARK,
    QUESTION_MARK,
    PRISION,
    TELEPORT,
    RAILROAD,
    COMPANY,
    TAXES,
    CHEST
} HSETYPE;

typedef enum{
    COLLECT_MONEY, 
    PAY_MONEY, 
    ESPECIAL_PAY, 
    MOVE_TO, 
    MOVE_BACK, 
    MOVE_AND_RECEIVE, 
    MOVE_NEAREST_RAILROAD, 
    MOVE_NEAREST_COMPANY, 
    GO_TO_JAIL, 
    GET_OUT_OF_JAIL,
    PAY_EACH_PLAYER,
    COLLECT_FROM_EACH_PLAYER, 
    MOVE_RECEIVE_IF 
} CARD_ACTION;

typedef enum{
    BANKRUPT,
    NEED_LIQUIDATION,
    CAN_PAY
} PAYMENT_STATUS;

typedef enum{
    NONE,
    BUY,
    AUCTION
} EVENT_ACTION;
#endif