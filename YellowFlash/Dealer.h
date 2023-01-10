#pragma once

#include "Deck.h"

typedef struct Dealer
{
	const char* Name;
	Deck* Deck;
}Dealer;

Dealer* Create_Dealer();