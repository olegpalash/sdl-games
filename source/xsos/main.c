#include <stdio.h>
#include "game.h"
#include "interface.h"
#include "ai.h"
#include "map.h"

int main(int argc, void argv)
{
	init_ui(768, 768);
	
	int cont = 1;
	int map[3][3] = 
	{
		{MAP_EMPTY, MAP_EMPTY, MAP_EMPTY},
		{MAP_EMPTY, MAP_EMPTY, MAP_EMPTY},
		{MAP_EMPTY, MAP_EMPTY, MAP_EMPTY}
	}
	
	while (cont)
	{
		player_turn(map);
		if (get_winner(map) != 0)
		{
			switch (get_winner(map))
			{
				case -1:
					puts("Ничья.");
					break;
				case 1:
					puts("Вы победили.");
					break;
				case 2:
					puts("ИИ победил.");
					break;
			}
			cont = 0;
		}
		
		ai_turn(map);
		if (get_winner(map) != 0)
		{
			switch (get_winner(map))
			{
				case -1:
					puts("Ничья.");
					break;
				case 1:
					puts("Вы победили.");
					break;
				case 2:
					puts("ИИ победил.");
					break;
			}
			cont = 0;
		}
	}
	
	return 0;
}

