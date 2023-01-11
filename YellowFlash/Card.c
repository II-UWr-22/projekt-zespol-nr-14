#include "Card.h"

#include <stdio.h>
#include <stdlib.h>

Card* Create_Card()
{
	Card* card = (Card*)malloc(sizeof(Card));

	return card;
}

Card* Create_Card(Value value, Suit suit)
{
	Card* card = (Card*)malloc(sizeof(Card));
	card->Value = value;
	card->Suit = suit;

	return card;
}

Card* Create_Card(Value value, Suit suit, bool isOpened)
{
	Card* card = (Card*)malloc(sizeof(Card));
	card->Value = value;
	card->Suit = suit;
	card->IsOpened = isOpened;

	return card;
}

void Print_Card(Card card)
{
	if (card.IsOpened)
	{
		int value = card.Value + 2;
		if (value < 11)
		{
			printf("%d", value);
		}
		else if (value == 11)
		{
			printf("Jack");
		}
		else if (value == 12)
		{
			printf("Queen");
		}
		else if (value == 13)
		{
			printf("King");
		}
		else if (value == 14)
		{
			printf("Ace");
		}

		if (card.Suit == Club)
		{
			printf(" Club");
		}
		else if (card.Suit == Diamond)
		{
			printf(" Diamond");
		}
		else if (card.Suit == Spade)
		{
			printf(" Spade");
		}
		else if (card.Suit == Heart)
		{
			printf(" Heart");
		}
	}
	else
	{
		printf("Closed");
	}
}

void Close_Card(Card* card)
{
	card->IsOpened = false;
}

void Open_Card(Card* card)
{
	card->IsOpened = true;
}

void Rotate_Card(Card* card)
{
	card->IsOpened = !(card->IsOpened);
}