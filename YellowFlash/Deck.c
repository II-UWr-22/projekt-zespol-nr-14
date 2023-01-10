#include "Deck.h"

#include <malloc.h>

Deck* CreateDeck()
{
    Deck* deck = (Deck*)malloc(sizeof(Deck));
    deck->Head = NULL;
    deck->Tail = NULL;
    deck->Count = 0;
    deck->Add = Add_Card;
    deck->Get = Get_Card;
    deck->Fill = Fill_Deck;
    deck->Clear = Clear_Deck;

    return deck;
}

void Add_Card(Deck* deck, Card card)
{
    deck->Count++;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->Card = card;
    new_node->Next = NULL;

    if (deck->Head == NULL)
    {
        deck->Head = new_node;
        deck->Tail = new_node;
    }
    else
    {
        deck->Tail->Next = new_node;
        deck->Tail = new_node;
    }
}

Card Get_Card(Deck* deck, int index)
{
    Node* head = deck->Head;
    for (int i = 0; i < index; i++)
    {
        head = head->Next;
    }
    
    return head->Card;
}

void Clear_Deck(Deck* deck)
{
    Node* nextNode = deck->Head->Next;
    for (int i = 0; i < deck->Count; i++)
    {
        free(deck->Head);
        deck->Head = nextNode;
        deck->Count--;
    }

    free(deck->Tail);
}

void Fill_Deck(Deck* deck)
{
    for (int i = 0; i < 52; i++)
    {
        Card card;
        card.Value = (Value)(i % 13);
        card.Suit = (Suit)(i / 13);
        deck->Add(deck, card);
    }
}