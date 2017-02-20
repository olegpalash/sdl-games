#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "interface.h"
#include "ai.h"
#include "map.h"

int main(int argc, char** argv)
{
	init_ui(768, 768);
	
	int** map = (int**) calloc(3, sizeof(int*));
	int i;
	for (i = 0; i < 3; i++)
	{
		map[i] = (int*) calloc(3, sizeof(int));
		int j;
		for (j = 0; j < 3; j++)
		{
			map[i][j] = MAP_EMPTY;
		}
	}
	
	int quit = 0;
	while (!quit)
	{
		player_turn(map, &quit);
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
			quit = 1;
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
			quit = 1;
		}
	}
	
	return 0;
}

