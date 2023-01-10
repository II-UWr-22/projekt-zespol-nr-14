#ifndef PLAYER_H_
#define PLAYER_H_

#include "card.h"
#include "stdint.h"

typedef struct player_t
{
    const char *name;
    uint32_t validCards;
    card_t cards[5]; 
    uint64_t balance;
    uint64_t bid;
} player_t;

#endif