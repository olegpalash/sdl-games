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
	int outcome = 0;
	while (!quit)
	{
		player_turn(map, &quit);
		outcome = get_winner(map);
		if (outcome != -1) break;
		
		ai_turn(map);
		outcome = get_winner(map);
		if (outcome != -1) break;
	}
	
	show_message(outcome, map);
	
	return 0;
}

