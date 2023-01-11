#pragma once

#include "Card.h"
#include "Combination.h"
#include "Cards_In_Hands.h"

typedef struct Player
{
	const char* Name;
	int Bank;
	Combination Combination;
	Cards_In_Hands* Cards;
	int Bet;
}Player;

Player* Create_Player();

Player* Create_Player(const char* name);

Player* Create_Player(const char* name, int bank);

void Print_Player(Player player);