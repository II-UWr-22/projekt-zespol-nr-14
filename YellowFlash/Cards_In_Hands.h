#pragma once

#include "Card.h"
#include "Node.h"

//typedef struct Node
//{
//	Card Card;
//	Node* Next;
//}Node;

typedef struct Cards_In_Hands
{
    Node* Head;
    Node* Tail;
    int Count;
    void (*Add)(Cards_In_Hands* head, Card card);
    Card(*Get)(Cards_In_Hands* head, int index);
    void (*Clear)(Cards_In_Hands* head);
}Cards_In_Hands;

Cards_In_Hands* Create_Cards_In_Hands();
void Add_Card_To_Hands(Cards_In_Hands* head, Card card);
Card Get_Card_From_Hands(Cards_In_Hands* head, int index);
void Clear_Hands(Cards_In_Hands* head);
