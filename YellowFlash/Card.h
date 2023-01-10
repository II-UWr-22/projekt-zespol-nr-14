#pragma once

#include "Suit.h"
#include "Value.h"
#include <stdbool.h>

typedef struct Card
{
	Value Value;
	Suit Suit;
	bool IsOpened;
}Card;

void Print_Card(Card card);

Card* Create_Card();

Card* Create_Card(Value value, Suit suit);

Card* Create_Card(Value value, Suit suit, bool isOpened);

void Close_Card(Card* card);
void Open_Card(Card* card);
void Rotate_Card(Card* card);