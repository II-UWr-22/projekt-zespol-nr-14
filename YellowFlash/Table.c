#include "Table.h"
#include <malloc.h>

Table* Create_Table()
{
	Table* table = (Table*)malloc(sizeof(Table));
	table->Cards = Create_Cards_In_Hands();
	table->Bank = 0;

	return table;
}