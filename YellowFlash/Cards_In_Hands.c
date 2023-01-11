#include "Cards_In_Hands.h"

#include <malloc.h>

Cards_In_Hands* Create_Cards_In_Hands()
{
    Cards_In_Hands* cards_In_Hands = (Cards_In_Hands*)malloc(sizeof(Cards_In_Hands));
    cards_In_Hands->Head = NULL;
    cards_In_Hands->Tail = NULL;
    cards_In_Hands->Count = 0;
    cards_In_Hands->Add = Add_Card_To_Hands;
    cards_In_Hands->Get = Get_Card_From_Hands;
    cards_In_Hands->Clear = Clear_Hands;

    return cards_In_Hands;
}

void Add_Card_To_Hands(Cards_In_Hands* list, Card card)
{
    list->Count++;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->Card = card;
    new_node->Next = NULL;

    if (list->Head == NULL)
    {
        list->Head = new_node;
        list->Tail = new_node;
    }
    else
    {
        list->Tail->Next = new_node;
        list->Tail = new_node;
    }
}

Card Get_Card_From_Hands(Cards_In_Hands* list, int index)
{
    Node* head = list->Head;
    for (int i = 0; i < index; i++)
    {
        head = head->Next;
    }

    return head->Card;
}

void Clear_Hands(Cards_In_Hands* list)
{
    Node* nextNode = list->Head->Next;
    for (int i = 0; i < list->Count; i++)
    {
        free(list->Head);
        list->Head = nextNode;
        list->Count--;
    }

    free(list->Tail);
}