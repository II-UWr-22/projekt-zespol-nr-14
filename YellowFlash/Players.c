#include "Players.h"

#include <malloc.h>

Players* Create_Players_List()
{
    Players* players = (Players*)malloc(sizeof(Players));
    players->Head = NULL;
    players->Tail = NULL;
    players->Count = 0;
    players->Add = Add_Player;
    players->Get = Get_Player;
    players->Clear = Clear_Players_List;

    return players;
}

void Add_Player(Players* list, Player* player)
{
    list->Count++;

    PlayerNode* new_node = (PlayerNode*)malloc(sizeof(PlayerNode));
    new_node->Player = player;
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

Player* Get_Player(Players* list, int index)
{
    PlayerNode* head = list->Head;
    for (int i = 0; i < index; i++)
    {
        head = head->Next;
    }

    return head->Player;
}

void Clear_Players_List(Players* list)
{
    PlayerNode* nextNode = list->Head->Next;
    for (int i = 0; i < list->Count; i++)
    {
        free(list->Head);
        list->Head = nextNode;
        list->Count--;
    }

    free(list->Tail);
}