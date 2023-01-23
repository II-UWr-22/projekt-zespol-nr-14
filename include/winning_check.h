//
// Created by osado on 23.01.2023.
//

#ifndef WINNING_CHECK_C_WINNING_CHECK_H
#define WINNING_CHECK_C_WINNING_CHECK_H

#include "card.h"
#include "player.h"

void winner_check(int numbers_of_players, player_t players[], card_t table_cards[5], int *how_many_players_win, int *table_of_winners);

#endif //WINNING_CHECK_C_WINNING_CHECK_H
