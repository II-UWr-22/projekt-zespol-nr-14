#pragma once

#include "Player.h"

typedef struct PlayerNode 
{
	Player* Player;
	PlayerNode* Next;
}PlayerNode;

typedef struct Players
{
    PlayerNode* Head;
    PlayerNode* Tail;
    int Count;
    void (*Add)(Players* head, Player* player);
    Player* (*Get)(Players* head, int index);
    void (*Clear)(Players* head);
}Players;

Players* Create_Players_List();
void Add_Player(Players* head, Player* player);
Player* Get_Player(Players* head, int index);
void Clear_Players_List(Players* head);