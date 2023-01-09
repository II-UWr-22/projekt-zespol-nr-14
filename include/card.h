
#ifndef CARD_H_
#define CARD_H_

typedef struct card_t
{
    int suit; // 0 - spade, 1 - heart, 2 - diamond, 3 - club
    int value; // 2-10, 11 - J, 12 - Q, 13 - K, 14 - A
} card_t;

#endif