#include "winning_check.h"
#include <stdio.h>
#include <stdlib.h>

void looking_for_winner(int p_amount, int max, int arr[], int player_score[],int *how_many_players_win, int *table_of_winners){
    int max_value = 0;
    for (int i = 0; i < p_amount; ++i) {
        if (player_score[i] == max && arr[i] > max_value) { max_value = arr[i]; }
    }
    for (int i = 0; i < p_amount; ++i) {
        if ( player_score[i] == max && arr[i] == max_value) {
            table_of_winners[*how_many_players_win] = i;
            *how_many_players_win += 1;
        }
    }
}

int serching_for_color(card_t *cards, int *highest_c_card) {
    int color[4];
    for (int i = 0; i < 4; ++i) {
        color[i] = 0;
    }
    for (int i = 0; i < 7; i++) {
        color[cards[i].suit] += 1;
    }
    for (int i = 0; i < 4; i++) {
        if (color[i] > 4) {
            for (int j = 0; j < 5; j++) {
                if (cards[j].suit == i) {
                    *highest_c_card = cards[j].value;
                    break;
                }
            }
            return 1;
        }
    }
    *highest_c_card = 0;
    return 0;
}// works
int fstrength_of_pair(card_t *card, int *power) {
    int count = 1;
    int value = 0;//is there 4 card set in ahnd
    int pairs[4];
    for (int i = 0; i < 4; ++i) {
        pairs[i] = 0;
    }

    int how_many_set = 0;
    for (int i = 0; i < 6; ++i) {
        if (card[i].value == card[i + 1].value) {
            count += 1;
        } else {
            if (count >= 2) {
                if (power[count - 1] == 0) { power[count - 1] = card[i - 1].value; }
                else {
                    if (power[count - 2] == 0) { power[count - 2] = card[i - 1].value; }
                }
                pairs[how_many_set] = count;
                how_many_set += 1;
                if (count >= 4) {
                    value = 1;
                }

            }
            count = 1;
        }
    }
    if (count >= 2) {
        how_many_set += 1;
        if (power[count - 1] == 0) { power[count - 1] = card[7 - 1].value; }
        else {
            if (power[count - 2] == 0) { power[count - 2] = card[7 - 1].value; }
        }
    }
    if (count == 4) { value = 1; }
    if (count < 2) { count = 0; }
    for (int i = 0; i < 3; ++i) {
        count += pairs[i];
    }
    if (count <= 5) {
        if (count != 4) {
            if (count < 4) { return count; }
            return count + 1;
        } else {
            if (value != 0) {
                return 7;
            } else return 2;
        }
    }
    if (count == 6) {
        if (value != 0) { return 7; }
        if (how_many_set == 2) { return 6; }
        else return 2;
    }
    return 0;
}

// compare for ascending values
int card_cmp(const void *a, const void *b)
{
    return -(int)( ((const card_t*)a)->value - ((const card_t*)b)->value );
}

int is_in_sequence(card_t *cards, int *start_of_sequence) {
    int count;
    *start_of_sequence = cards[0].value;
    for (int i = 0; i < 3; ++i) {
        count = 1;
        for (int j = 0; j < 6; ++j) {
            if (cards[j].value == cards[j + 1].value + 1) {
                count += 1;
                if (count > 4) { return 1; }
            }
            if (cards[j].value != cards[j + 1].value + 1 && cards[j].value != cards[j + 1].value) {
                *start_of_sequence = cards[j + 1].value;
                count = 1;
            }
        }
    }
    if (count == 4 && cards[6].value == 2 && cards[0].value == 14) { return 1; }// check if ace takes part in set
    return 0;
}

void winner_check(int numbers_of_players, player_t players[], card_t table_cards[5],
                  int *how_many_players_win, int *table_of_winners) {
    int player_score[numbers_of_players];//players score
    for (int i = 0; i < numbers_of_players; ++i) {
        player_score[i] = 0;
    }
    int highest_cart_value[numbers_of_players];//highest cart in players hand// include table carts important in looking for winer without any set
    int start_of_sequence[numbers_of_players];//cart that start sequence of "strit" // looking for winner
    int power_of_pair_cards[numbers_of_players][4];//power of cart that make sets of pair or triple
    int highest_color_card[numbers_of_players];//highest card in color set/ if therw is not any color - 0
    for (int i = 0; i < numbers_of_players; ++i) {
        for (int j = 0; j < 4; ++j) {
            power_of_pair_cards[i][j] = 0;
        }
    }
    for (int i = 0; i < numbers_of_players; ++i) {
        if( players[i].validCards == 0 )
        {
            player_score[i] = -1;
            continue;
        }

        card_t player_card[7];
        for (int j = 0; j < 5; ++j) {
            player_card[j] = table_cards[j];
        }
        player_card[5] = players[i].cards[0];
        player_card[6] = players[i].cards[1];
        
        qsort(player_card, 7, sizeof(card_t), card_cmp);

        highest_cart_value[i] = player_card[0].value;
        int is_sequence = is_in_sequence(player_card, &start_of_sequence[i]);
        int is_color = serching_for_color(player_card, &highest_color_card[i]);
        int strength_of_pair = fstrength_of_pair(player_card, power_of_pair_cards[i]);
        if (is_color && is_sequence) {
            player_score[i] = 8;
            continue;
        }
        if (is_color) {
            if (strength_of_pair < 4) {
                player_score[i] = 5;
                continue;
            }

            player_score[i] = strength_of_pair + 2;
            continue;
        }
        if (is_sequence) { player_score[i] = 4; } else { player_score[i] = strength_of_pair; }
    }
    int count = 0;
    int how_many_player_with_this_same_amount_of_points = 0;
    int max = 0;
    for (int i = 0; i < numbers_of_players; ++i) {
        if (player_score[i] > max) {
            max = player_score[i];
            continue;
        }
        if (player_score[i] == max) {
            count += 1;
            if (how_many_player_with_this_same_amount_of_points <
                count) { how_many_player_with_this_same_amount_of_points = count; }
        } else count = 0;
    }
    
    if( max == -1 ) return;

    if (how_many_player_with_this_same_amount_of_points == 0) {
        for (int i = 0; i < numbers_of_players; ++i) {
            if (player_score[i] == max) {
                table_of_winners[*how_many_players_win] = i;
                *how_many_players_win += 1;
                return;
            }
        }
    }

    if (max == 5) {
        looking_for_winner(numbers_of_players,max,highest_color_card,player_score,how_many_players_win,table_of_winners);
        return;
    }
    if (max == 0) {
        looking_for_winner(numbers_of_players,max,highest_cart_value,player_score,how_many_players_win,table_of_winners);
        return;
    }
    if (max == 8 || max == 4) {
        looking_for_winner(numbers_of_players,max,start_of_sequence,player_score, how_many_players_win, table_of_winners);
        return;
    }
    if (max == 1 || max == 3 || max == 7) {
        int max_value = 0;
        int place;
        if (max == 3) { place = 2; }
        if (max == 7) { place = 3; }
        if (max == 1) { place = 1; }
        for (int i = 0; i < numbers_of_players; ++i) {
            if (player_score[i] == max &&
                power_of_pair_cards[i][place] > max_value) { max_value = power_of_pair_cards[i][place]; }
        }
        for (int i = 0; i < numbers_of_players; ++i) {
            if (power_of_pair_cards[i][place] == max_value && player_score[i] == max) {
                table_of_winners[*how_many_players_win] = i;
                *how_many_players_win += 1;
            }
        }
        return;
    }
    if (max == 2 || max == 6) {
        int place1;
        int place2;
        int max1 = 0;
        int max2 = 0;
        if (max == 2) {
            place1 = 1;
            place2 = 0;
        }
        if (max == 6) {
            place1 = 2;
            place2 = 1;
        }
        for (int i = 0;i < numbers_of_players;i++) {
            if (player_score[i] == max && power_of_pair_cards[i][place1] >= max1) {
                max2 = power_of_pair_cards[i][place2];
                max1 = power_of_pair_cards[i][place1];
            }
        }
        for (int i = 0;i < numbers_of_players;++i) {
            if (player_score[i] == max && power_of_pair_cards[i][place1] == max1 &&
                power_of_pair_cards[i][place2] == max2) {
                table_of_winners[*how_many_players_win] = i;
                *how_many_players_win += 1;
            }
        }
        return;
    }
}

