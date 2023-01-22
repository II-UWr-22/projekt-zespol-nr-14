//
// Created by osado on 21.01.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include "player.h"

int serching_for_color(card_t *carts, int *highest_c_cart) {
    int color[4];
    for (int i = 0; i < 4; ++i) {
        color[i]=0;
    }
    for (int i = 0; i < 7; i++) {
        color[carts[i].suit] += 1;
    }
    for (int i = 0; i < 4; i++) {
        if (color[i] > 4) {
            for (int j = 0; j < 5; j++) {
                if (carts[j].suit == i) {
                    highest_c_cart[0] = carts[j].value;
                    break;
                }
            }
            return 1;
        }
    }
    highest_c_cart[0] = 0;
    return 0;
}// works
int fstrength_of_pair(card_t *card, int *power) {
    int count = 1;
    int value = 0;//is there 4 cart set in ahnd
    int pairs[4];
    for (int i = 0; i < 4; ++i) {
        pairs[i]=0;
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
            if (count<4){return count;}
            return count+1;
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
}//
void buble_sort_for_carts(card_t *carts) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (carts[j].value < carts[j + 1].value) {
                card_t par = carts[j + 1];
                carts[j + 1] = carts[j];
                carts[j] = par;
            }
        }
    }
int is_in_sequence(card_t *carts, int *start_of_sequence) {
    int count;
    start_of_sequence[0] = carts[0].value;
    for (int i = 0; i < 3; ++i) {
        count = 1;
        for (int j = 0; j < 6; ++j) {
            if (carts[j].value == carts[j + 1].value + 1) {
                count += 1;
                if (count > 4) { return 1; }
            }
            if (carts[j].value != carts[j + 1].value + 1 && carts[j].value != carts[j + 1].value) {
                start_of_sequence[0] = carts[j + 1].value;
                count = 1;
            }
        }
    }
    if (count == 4 && carts[6].value == 2 && carts[0].value == 14) { return 1; }// check if ace take part in set
    return 0;
}

player_t *winner_check(player_t *players, int numbers_of_player, card_t table_carts[5], int *how_many_players_win, player_t *table_of_winners ) {
    int player_score[numbers_of_player];//players score
    for (int i = 0; i < numbers_of_player; ++i) {
        player_score[i]=0;
    }
    int highest_cart_value[numbers_of_player];//highest cart in players hand// include table carts important in looking for winer without any set
    int start_of_sequence[numbers_of_player];//cart that start sequence of "strit" // looking for winner
    int power_of_pair_carts[numbers_of_player][4];//power of cart that make sets of pair or triple
    int highest_color_cart[numbers_of_player];//highest card in color set/ if therw is not any color - 0
    for (int i = 0; i < numbers_of_player; ++i) {
        for (int j = 0; j < 4; ++j) {
            power_of_pair_carts[i][j] = 0;
        }
    }
    for (int i = 0; i < numbers_of_player; ++i) {
        card_t player_cart[7];
        for (int j = 0; j < 5; ++j) {
            player_cart[j] = table_carts[j];
        }
        player_cart[5] = players[i].cards[0];
        player_cart[6] = players[i].cards[1];
        buble_sort_for_carts(player_cart);
        highest_cart_value[i] = player_cart[0].value;
        int is_sequence = is_in_sequence(player_cart, &start_of_sequence[i]);
        int is_color = serching_for_color(player_cart, &highest_color_cart[i]);
        int strength_of_pair = fstrength_of_pair(player_cart, &power_of_pair_carts[i][0]);
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
    for (int i = 0; i < numbers_of_player ; ++i) {
        if (player_score[i] > max) { max = player_score[i];continue; }
        if (player_score[i] == max) {
            count += 1;
            if (how_many_player_with_this_same_amount_of_points < count) { how_many_player_with_this_same_amount_of_points = count; }
        } else count = 0;
    }
    if (how_many_player_with_this_same_amount_of_points == 0) {
        for (int i = 0; i < numbers_of_player; ++i) {
            if (player_score[i] == max) {
                table_of_winners[0] = players[i];
                how_many_players_win[0]+=1;
                return table_of_winners;
            }
        }
    }
    int max_value = 0;
    short par = 0;
    if (max == 5) {
        for (int x = 0; x < numbers_of_player; x++) {
            if (player_score[x] == max && highest_color_cart[x] > max_value) { max_value = highest_color_cart[x];}
        }
        for (int i = 0; i < numbers_of_player; ++i) {
            if (highest_color_cart[i] == max_value && player_score[i] == max) {
                table_of_winners[par] = players[i];
                par += 1;
                how_many_players_win[0]+=1;
            }
        }
        return table_of_winners;
    }
    if (max == 0) {
        for (int x = 0; x < numbers_of_player; x++) {
            if (player_score[x] == max && highest_cart_value[x] > max_value) { max_value = highest_cart_value[x]; }
        }
        for (int i = 0; i < numbers_of_player; ++i) {
            if (highest_cart_value[i] == max_value && player_score[i] == max) {
                table_of_winners[par] = players[i];
                par += 1;
                how_many_players_win[0]+=1;
            }
        }
        return table_of_winners;
    }
    if (max == 8 || max == 4) {
        for (int i = 0; i < numbers_of_player; ++i) {
            if (player_score[i] == max && start_of_sequence[i] > max_value) { max_value = start_of_sequence[i]; }
        }
        for (int i = 0; i < numbers_of_player; ++i) {
            if (start_of_sequence[i] == max_value && player_score[i] == max) {
                table_of_winners[par] = players[i];
                par += 1;
                how_many_players_win[0]+=1;
            }
        }
        return table_of_winners;
    }
    if (max==1|| max==3 || max==7){
        int place;
        if(max==3){place=2;}
        if(max==7){place=3;}
        if(max==1){place=1;}
        for (int i = 0; i < numbers_of_player; ++i) {
            if (player_score[i] == max && power_of_pair_carts[i][place] > max_value) { max_value = power_of_pair_carts[i][place]; }
        }
        for (int i = 0; i < numbers_of_player; ++i) {
            if (power_of_pair_carts[i][place] == max_value && player_score[i] == max) {
                table_of_winners[par] = players[i];
                par += 1;
                how_many_players_win[0]+=1;
            }
        }
        return table_of_winners;
    }
    if(max==2 || max==6){
        int place1;
        int place2;
        int max1=0;
        int max2=0;
        if(max==2){place1=1;place2=0;}
        if(max==6){place1=2;place2=1;}
        for(int i =0;i<numbers_of_player;i++){
            if (player_score[i]==max && power_of_pair_carts[i][place1]>=max1){
                if(power_of_pair_carts[i][place2]>max2){max2=power_of_pair_carts[i][place2];}
                max1=power_of_pair_carts[i][place1];
            }
        }
        for (int i = 0; i < numbers_of_player; ++i) {
            if(player_score[i]==max && power_of_pair_carts[i][place1]==max1 && power_of_pair_carts[i][place2]==max2 ){
                table_of_winners[par] = players[i];
                par += 1;
                how_many_players_win[0]+=1;
            }
        }
        return table_of_winners;
    }
    return NULL;//something gone wrong
}
