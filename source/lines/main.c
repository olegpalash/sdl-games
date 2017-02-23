#include <stdlib.h>
#include "interface.h"
#include "game.h"

int main(void)
{
	int** map = calloc(10, sizeof(int*));

	int i, j;
	for (i = 0; i < 10; i++)
	{
		map[i] = calloc(10, sizeof(int));
		for (j = 0; j < 10; j++)
		{
			map[i][j] = 0;
		}
	}
	
	add_balls(map, 3);
	
	init_ui(500, 500);
	
	int quit = 0;
	int score = 0;
	while (!quit)
	{
		player_turn(map, &quit);
		
		int a = get_lines(map);
		if (a == 0)
		{
			if (!add_balls(map, 3))
			{
				quit = 1;
			}
		}
		else
		{
			score += a;
		}
	}
	
	show_message(score, map);
	close_ui();
	
	return 0;	
}

