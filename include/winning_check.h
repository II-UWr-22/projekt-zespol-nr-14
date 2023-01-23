//
// Created by osado on 23.01.2023.
//

#ifndef WINNING_CHECK_C_WINNING_CHECK_H
#define WINNING_CHECK_C_WINNING_CHECK_H

#include "card.h"
#include "player.h"

void looking_for_winner(int p_amount, int max, int arr[], int player_score[],int *how_many_players_win, int *table_of_winners);

int serching_for_color(card_t *cards, int *highest_c_card);

int fstrength_of_pair(card_t *card, int *power);

void buble_sort_for_carts(card_t *cards);

int is_in_sequence(card_t *cards, int *start_of_sequence);

void winner_check(int numbers_of_players, player_t players[], card_t table_cards[5], int *how_many_players_win, int *table_of_winners);

#endif //WINNING_CHECK_C_WINNING_CHECK_H
