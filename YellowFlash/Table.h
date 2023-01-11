#pragma once

#include "Cards_In_Hands.h"

typedef struct Table
{
	int Bank;
	Cards_In_Hands* Cards;
}Table;

Table* Create_Table();
