#include "Dealer.h"
#include <malloc.h>

Dealer* Create_Dealer()
{
	Dealer* dealer = (Dealer*)malloc(sizeof(Dealer));
	dealer->Name = "Dealer";
	dealer->Deck = CreateDeck();

	return dealer;
}