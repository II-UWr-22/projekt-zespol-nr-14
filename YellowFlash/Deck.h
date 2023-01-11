#pragma once

#include "Card.h"
#include "Node.h"
//typedef struct Node
//{
//    Card Card;
//    Node* Next;
//}Node;

typedef struct Deck
{
    Node* Head;
    Node* Tail;
    int Count;
    void (*Add)(Deck* deck, Card card);
    Card(*Get)(Deck* deck, int index);
    void (*Fill)(Deck* deck);
    void (*Clear)(Deck* deck);
}Deck;

Deck* CreateDeck();
void Add_Card(Deck* head, Card card);
Card Get_Card(Deck* head, int index);
void Clear_Deck(Deck* head);
void Fill_Deck(Deck* deck);