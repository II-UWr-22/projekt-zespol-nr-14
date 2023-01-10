#include "Player.h"
#include <malloc.h>
#include <stdio.h>

Player* Create_Player()
{
	/*Player* player = (Player*)malloc(sizeof(Player));
	player->Name = "Empty";
	player->Bank = 0;
	player->Cards = Create_Cards_In_Hands();
	player->Bet = 0;

	return player;*/

	return Create_Player("Empty");
}

Player* Create_Player(const char* name)
{
	/*Player* player = (Player*)malloc(sizeof(Player));
	player->Name = name;
	player->Bank = 0;
	player->Cards = Create_Cards_In_Hands();
	player->Bet = 0;

	return player;*/

	return Create_Player(name, 0);
}

Player* Create_Player(const char* name, int bank)
{
	Player* player = (Player*)malloc(sizeof(Player));
	player->Name = name;
	player->Bank = bank;
	player->Cards = Create_Cards_In_Hands();

	player->Bet = 0;

	return player;
}

void Print_Player(Player player)
{
	printf("Name: %s Bank: %d", player.Name, player.Bank);
}