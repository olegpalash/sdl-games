#include "map.h"

typedef struct
{
	int i;
	int j;
	int di;
	int dj;
} pass;

static pass passes[8] = 
{
	{0, 0, 1, 1},	// диагонали
	{0, 2, 1, -1},
	{0, 0, 0, 1},	// по горизонтали
	{1, 0, 0, 1},
	{2, 0, 0, 1},
	{0, 0, 1, 0},	// по вертикали
	{0, 1, 1, 0},
	{0, 2, 1, 0}
};

int ai_turn(int** map)
{
	int n;
	int lines[8][3];
	
	for (n = 0; n < 8; n++)
	{
		lines[n][0] = 0;
		lines[n][1] = 0;
		
		int i = passes[n].i;
		int j = passes[n].j;
		int m;		
		
		for (m = 0; m < 3; m++)
		{
			if (map[i][j] == MAP_ZERO)
			{
				lines[n][0]++;
			}
			else if (map[i][j] == MAP_CROSS)
			{
				lines[n][1]++;
			}
						
			i += passes[n].di;
			j += passes[n].dj;
		}
	}
	
	for (n = 0; n < 8; n++)
	{
		lines[n][2] = 0;

		if (lines[n][0]+lines[n][1] == 3) continue;
		
		if (lines[n][0] == 2) lines[n][2] = 5;
		else if (lines[n][1] == 2) lines[n][2] = 4;
		else lines[n][2] = 1;
	}
	
	int max = 0;
	for (n = 1; n < 8; n++)
	{
		if (lines[n][2] > lines[max][2]) max = n;
	}
	
	int i = passes[max].i;
	int j = passes[max].j;
			
	for (n = 0; n < 3; n++)
	{
		if (map[i][j] == MAP_EMPTY)
		{
			map[i][j] = MAP_ZERO;
			break;
		}
		
		i += passes[max].di;
		j += passes[max].dj;
	}
	
	return 1;
}

